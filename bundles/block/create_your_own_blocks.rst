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

    In this example, we create an ``RssBlock``. This particular block type
    already exists in the Symfony2 CMF BlockBundle, so you can also look at
    the end result if you want.

Create a block document
-----------------------

The first thing you need is an document that holds the data. It is
recommended to extend ``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\AbstractBlock``.
You are of course free to do your own document, but need to at least implement
``Sonata\BlockBundle\Model\BlockInterface``. In your document, you
need to define the ``getType`` method which returns the type name of your block,
for instance ``acme_main.block.rss``.

.. code-block:: php

    // src/Acme/MainBundle/Document/RssBlock.php
    namespace Acme\MainBundle\Document;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCRODM;
    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\AbstractBlock;

    /**
     * Rss Block
     *
     * @PHPCRODM\Document(referenceable=true)
     */
    class RssBlock extends AbstractBlock
    {
        public function getType()
        {
            return 'acme_main.block.rss';
        }
    }

Create a Block Service
----------------------

You could choose to use a an already existing block service because the
configuration and logic already satisfy your needs. For our RSS block we
create a service that knows how to handle ``RssBlocks``:

* The method ``setDefaultSettings`` configures a template, title, url and the
  maximum amount of items::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;
    use Symfony\Component\HttpFoundation\Response;
    use Sonata\BlockBundle\Block\BaseBlockService;
    use Sonata\BlockBundle\Block\BlockContextInterface;

    class RssBlockService extends extends BaseBlockService
    {
        // ...
        public function setDefaultSettings(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'template' => 'AcmeMainBundle::Block::block_rss.html.twig',
                'url'      => false,
                'title'    => 'Feed items',
                'maxItems' => 10,
            ));
        }
        // ...

        public function execute(BlockContextInterface $blockContext, Response $response = null)
        {
            /** @var $block RssBlock */
            $block = $blockContext->getBlock();

            if (!$block->getEnabled()) {
                return new Response;
            }

            $requestParams = $block->resolveRequestParams($this->request, $blockContext);

            return new Response($this->renderer->render(new ControllerReference(
                    'acme_main.controller.rss:block',
                    $requestParams
                )
            ));
        }
    }

The execute method passes the settings to an RSS reader service and forwards
the feed items to a template. (See :ref:`bundle-block-execute` for more on the
block service ``execute`` method).

Define the service in a configuration file. It is important to tag your BlockService
with ``sonata.block`` to make it known to the SonataBlockBundle.

.. configuration-block::

    .. code-block:: yaml

        acme_main.rss_reader:
            class: Acme\MainBundle\Feed\SimpleReader

        sandbox_main.block.rss:
            class: Acme\MainBundle\Block\RssBlockService
            arguments:
                - "acme_main.block.rss"
                - "@templating"
                - "@sonata.block.renderer"
                - "@acme_main.rss_reader"
            tags:
                - {name: "sonata.block"}

    .. code-block:: xml

        <service id="acme_main.rss_reader" class="Acme\MainBundle\Feed\SimpleReader" />

        <service id="sandbox_main.block.rss" class="Acme\MainBundle\Block\RssBlockService">
            <tag name="sonata.block" />

            <argument>acme_main.block.rss</argument>
            <argument type="service" id="templating" />
            <argument type="service" id="sonata.block.renderer" />
            <argument type="service" id="acme_main.block.rss_reader" />
        </service>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->register('acme_main.rss_reader', 'Acme\MainBundle\Feed\SimpleReader');

        $container
            ->addDefinition('sandbox_main.block.rss', new Definition(
                'Acme\MainBundle\Block\RssBlockService',
                array(
                    'acme_main.block.rss',
                    new Reference('templating'),
                    new Reference('sonata.block.renderer'),
                    new Reference('acme_main.rss_reader'),
                )
            ))
            ->addTag('sonata.block')
        ;
