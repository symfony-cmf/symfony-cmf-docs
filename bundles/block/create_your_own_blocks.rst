Create your own Blocks
======================

Follow these steps to create a block:

* define a block document class;
* if needed, create a block service and declare it (optional);
* instantiate a data object representing your block in the repository, see
  :ref:`bundle-block-document`;
* render the block, see :ref:`bundle-block-rendering`;

Lets say you are working on a project where you have to integrate data
received from several RSS feeds.  Of course you could create an ActionBlock
for each of these feeds, but wouldn't this be silly? In fact, all those actions
would look similar: Receive data from a feed, sanitize it and pass the data to
a template. So instead you decide to create your own block, the ``RssBlock``.

.. tip::

    In this example, you create an ``RssBlock``. An RSS block already exists in
    the CmfBlockBundle, but this one is more powerful as it allows to define a
    specific feed URL per block instance.

Create a block document
-----------------------

The first thing you need is an document that contains the options and indicates
the location where the RSS feed should be shown. The easiest way is to extend
``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\AbstractBlock``, but you are
free to do create your own document. At least, you have to implement
``Sonata\BlockBundle\Model\BlockInterface``. In your document, you
need to define the ``getType`` method which returns the type name of your block,
for instance ``acme_main.block.rss``::

    // src/Acme/MainBundle/Document/RssBlock.php
    namespace Acme\MainBundle\Document;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;
    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\AbstractBlock;

    /**
     * @PHPCR\Document(referenceable=true)
     */
    class RssBlock extends AbstractBlock
    {
        /**
         * @PHPCR\String(nullable=true)
         */
        private $feedUrl;

        /**
         * @PHPCR\String()
         */
        private $title;

        public function getType()
        {
            return 'acme_main.block.rss';
        }

        public function getOptions()
        {
            $options = array(
                'title' => $this->title,
            );
            if ($this->feedUrl) {
                $options['url'] = $this->feedUrl;
            }

            return $options;
        }

        // Getters and setters for title and feedUrl...
    }

Create a Block Service
----------------------

If the configuration and logic already satisfies your needs, you should use an
already existing block service. For your RSS block, you need a custom service
that knows how to fetch the feed data of an ``RssBlock``::

    // src/Acme/MainBundle/Block/RssBlockService.php
    namespace Acme\MainBundle\Block;

    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    use Sonata\AdminBundle\Form\FormMapper;
    use Sonata\AdminBundle\Validator\ErrorElement;

    use Sonata\BlockBundle\Model\BlockInterface;
    use Sonata\BlockBundle\Block\BlockContextInterface;
    use Sonata\BlockBundle\Block\BaseBlockService;

    class RssBlockService extends BaseBlockService
    {
        public function getName()
        {
            return 'Rss Reader';
        }

        /**
         * Define valid options for a block of this type.
         */
        public function setDefaultSettings(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'url'      => false,
                'title'    => 'Feed items',
                'template' => 'AcmeMainBundle:Block:rss.html.twig',
            ));
        }

        /**
         * The block context knows the default settings, but they can be
         * overwritten in the call to render the block.
         */
        public function execute(BlockContextInterface $blockContext, Response $response = null)
        {
            if (!$block->getEnabled()) {
                return new Response();
            }

            // merge settings with those of the concrete block being rendered
            $settings = $blockContext->getSettings();
            $resolver = new OptionsResolver();
            $resolver->setDefaults($settings);
            $settings = $resolver->resolve($block->getOptions());

            $feeds = false;
            if ($settings['url']) {
                $options = array(
                    'http' => array(
                        'user_agent' => 'Sonata/RSS Reader',
                        'timeout' => 2,
                    )
                );

                // retrieve contents with a specific stream context to avoid php errors
                $content = @file_get_contents($settings['url'], false, stream_context_create($options));

                if ($content) {
                    // generate a simple xml element
                    try {
                        $feeds = new \SimpleXMLElement($content);
                        $feeds = $feeds->channel->item;
                    } catch (\Exception $e) {
                        // silently fail error
                    }
                }
            }

            return $this->renderResponse($blockContext->getTemplate(), array(
                'feeds'     => $feeds,
                'block'     => $blockContext->getBlock(),
                'settings'  => $settings
            ), $response);
        }

        // These methods are required by the sonata block service interface.
        // They are not used in the CMF. To edit, create a sonata admin or
        // something else that builds a form and collects the data.

        public function buildEditForm(FormMapper $formMapper, BlockInterface $block)
        {
            throw new \Exception();
        }

        public function validateBlock(ErrorElement $errorElement, BlockInterface $block)
        {
            throw new \Exception();
        }
    }

The execute method simply reads the RSS feed and forwards the items to
a template. See :ref:`bundle-block-execute` for more information on the
``execute`` method of the block service. To not make your page very slow
by fetching the feed on each request, have a look at the
:doc:`cache documentation <cache>`.

To make the block work, the last step is to define the service. Do not forget
to tag your service with ``sonata.block`` to make it known to the
SonataBlockBundle. The first argument is the name of the block this service
handles, as per the ``getType`` method of the block. The second argument is the
templating, in order to be able to render this block.

.. configuration-block::

    .. code-block:: yaml

        sandbox_main.block.rss:
            class: Acme\MainBundle\Block\RssBlockService
            arguments:
                - "acme_main.block.rss"
                - "@templating"
            tags:
                - {name: "sonata.block"}

    .. code-block:: xml

        <service id="sandbox_main.block.rss" class="Acme\MainBundle\Block\RssBlockService">
            <tag name="sonata.block" />

            <argument>acme_main.block.rss</argument>
            <argument type="service" id="templating" />
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container
            ->addDefinition('sandbox_main.block.rss', new Definition(
                'Acme\MainBundle\Block\RssBlockService',
                array(
                    'acme_main.block.rss',
                    new Reference('templating'),
                )
            ))
            ->addTag('sonata.block')
        ;
