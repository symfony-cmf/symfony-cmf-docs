Create your own Blocks
======================

Follow these steps to create a block:

* define a block document class;
* if needed, create a block service and declare it (optional);
* instantiate a data object representing your block in the repository, see
  :ref:`bundles-block-document`;
* render the block, see :ref:`bundles-block-rendering`;

Lets say you are working on a project where you have to integrate data
received from several RSS feeds. You could create an ActionBlock for each of
these feeds, but all those actions would look similar: Receive data from a
feed, sanitize it and pass the data to a template. This would be code
duplication, so instead you decide to create your own block, the ``RssBlock``.

.. tip::

    In this example, you create an ``RssBlock``. An RSS block already exists in
    the CmfBlockBundle, but this one is more powerful as it allows to define a
    specific feed URL per block instance.

Create a Block Document
-----------------------

The first thing you need is a document that contains the options and indicates
the location where the RSS feed should be shown. The easiest way is to extend
``Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\AbstractBlock``, but you are
free to do create your own document as long as it implements
``Sonata\BlockBundle\Model\BlockInterface``. In your document, you
need to define the ``getType`` method which returns the type name of your block,
for instance ``rss_block``::

    // src/AppBundle/Document/RssBlock.php
    namespace AppBundle\Document;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

    use Symfony\Cmf\Bundle\BlockBundle\Doctrine\Phpcr\AbstractBlock;

    /**
     * @PHPCR\Document(referenceable=true)
     */
    class RssBlock extends AbstractBlock
    {
        /**
         * @PHPCR\Field(type="string", nullable=true)
         */
        private $feedUrl;

        /**
         * @PHPCR\Field(type="string")
         */
        private $title;

        public function getType()
        {
            return 'app.rss_block';
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

The block service is responsible for rendering the blocks of a specific type,
as returned by the ``getType`` method.

A block service contains:

* The ``load`` method to initialize a block before rendering;
* The ``execute`` method to render the block;
* The ``setDefaultSettings`` to define default settings;
* Cache configuration;
* Form configuration;
* JavaScript and Stylesheet files to be loaded.

If the code of an existing service class satisfies your needs, you can simply
create a new service (see next section) with that existing class. The block
services provided by the CmfBlockBundle are in the namespace
``Symfony\Cmf\Bundle\BlockBundle\Block``.

For your RSS block, you need a custom service
that knows how to fetch the feed data of an ``RssBlock``::

    // src/AppBundle/Block/RssBlockService.php
    namespace AppBundle\Block;

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
                // template is at app/Resources/views/block/rss.html.twig
                'template' => 'block/rss.html.twig',
            ));
        }

        /**
         * The block context knows the default settings, but they can be
         * overwritten in the call to render the block.
         */
        public function execute(BlockContextInterface $blockContext, Response $response = null)
        {
            $block = $blockContext->getBlock();

            if (!$block->getEnabled()) {
                return new Response();
            }

            // merge settings with those of the concrete block being rendered
            $settings = $blockContext->getSettings();
            $resolver = new OptionsResolver();
            $resolver->setDefaults($settings);
            $settings = $resolver->resolve($block->getSettings());

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
        // They are not used in the CMF. To edit, create a symfony form or
        // a sonata admin.

        public function buildEditForm(FormMapper $formMapper, BlockInterface $block)
        {
            throw new \Exception();
        }

        public function validateBlock(ErrorElement $errorElement, BlockInterface $block)
        {
            throw new \Exception();
        }
    }

.. _bundles-block-execute:

The Execute Method
~~~~~~~~~~~~~~~~~~

This method of the block service contains *controller* logic. It is called
by the block framework to render a block. In this example, it checks if the
block is enabled and if so renders RSS items with a template.

.. note::

    If you need complex logic to handle a block, it is recommended to move that
    logic into a dedicated service and inject that service into the block
    service and defer execution in the ``execute`` method, passing along
    arguments determined from the block.

.. tip::

    When you do a block that will be slow to render, like this example where
    we read an RSS feed, you should activate :doc:`block caching <cache>`.

Default Settings
~~~~~~~~~~~~~~~~

The method ``setDefaultSettings`` allows your service to provide default
configuration options for a block. Settings can be altered in multiple
places afterwards, cascading as follows:

* Default settings from the block service;
* If you use a 3rd party bundle you might want to change them in the bundle
  configuration for your application see :ref:`bundles-block-configuration`;
* Settings can be altered through template helpers (see example below);
* And settings can also be altered in a block document. Do this only for
  settings that are individual to the specific block instance rather than
  all blocks of a type. These settings will be stored in the database.

Example of how settings can be overwritten through a template helper:

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_render({'name': 'rssBlock'}, {
            'title': 'Symfony2 CMF news',
            'url': 'http://cmf.symfony.com/news.rss'
        }) }}

    .. code-block:: html+php

        <?php $view['blocks']->render(array('name' => 'rssBlock'), array(
            'title' => 'Symfony2 CMF news',
            'url'   => 'http://cmf.symfony.com/news.rss',
        )) ?>

The Load Method
~~~~~~~~~~~~~~~

The method ``load`` can be used to load additional data into a block. It is
called each time a block is rendered before the ``execute`` method is called.

Form Configuration
~~~~~~~~~~~~~~~~~~

The methods ``buildEditForm`` and ``buildCreateForm`` specify how to build the
the forms for editing using a front-end or backend UI. The method
``validateBlock`` contains the validation configuration. This is not used in
the CMF and it is recommended to instead build forms or Sonata admin classes
that can handle the block documents.

Cache Configuration
~~~~~~~~~~~~~~~~~~~

The method ``getCacheKeys`` contains cache keys to be used for caching the
block. See the section :doc:`block cache <cache>` for more on caching.

JavaScripts and Stylesheets
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The methods ``getJavaScripts`` and ``getStylesheets`` of the service class
define the JavaScript and Stylesheet files needed by a block. There is a
Twig function and a templating helper to render all links for all blocks used
on the current page:

.. configuration-block::

    .. code-block:: jinja

        {{ sonata_block_include_javascripts("all") }}
        {{ sonata_block_include_stylesheets("all") }}

    .. code-block:: html+php

        <?php $view['blocks']->includeJavaScripts('all') ?>
        <?php $view['blocks']->includeStylesheets('all') ?>

.. note::

    This mechanism is not recommended. For optimal load times, it is better
    to have a central assets definition for your project and aggregate them
    into a single Stylesheet and a single JavaScript file, rather than having
    individual ``<link>`` and ``<script>`` tags for each single file.

Register the Block Service
--------------------------

To make the block work, the last step is to define the service. Do not forget
to tag your service with ``sonata.block`` to make it known to the
SonataBlockBundle. The first argument is the name of the block this service
handles, as per the ``getType`` method of the block. The second argument is the
``templating`` service, in order to be able to render this block.

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            app.rss_block:
                class: AppBundle\Block\RssBlockService
                arguments:
                    - "app.rss_block"
                    - "@templating"
                tags:
                    - { name: "sonata.block" }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <service id="app.rss_block" class="AppBundle\Block\RssBlockService">
            <tag name="sonata.block" />

            <argument>app.rss_block</argument>
            <argument type="service" id="templating" />
        </service>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container
            ->addDefinition('app.rss_block', new Definition(
                'AppBundle\Block\RssBlockService',
                array(
                    'app.rss_block',
                    new Reference('templating'),
                )
            ))
            ->addTag('sonata.block')
        ;
