The Backend - Sonata Admin
--------------------------

In this chapter you will build an administration interface with the help
of the SonataDoctrinePHPCRAdminBundle_.

Installation
~~~~~~~~~~~~

Ensure that you have the following package installed:

.. code-block:: javascript

    {
        ...
        require: {
            ...
            "sonata-project/doctrine-phpcr-admin-bundle": "1.0.*",
        },
        ...
    }

Enable the Sonata related bundles to your kernel::

    // app/AppKernel.php
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Sonata\BlockBundle\SonataBlockBundle(),
                new Sonata\jQueryBundle\SonatajQueryBundle(),
                new Knp\Bundle\MenuBundle\KnpMenuBundle(),
                new Sonata\DoctrinePHPCRAdminBundle\SonataDoctrinePHPCRAdminBundle(),
                new Sonata\AdminBundle\SonataAdminBundle(),
                new Sonata\CoreBundle\SonataCoreBundle(),
            );

            // ...
        }
    }

Sonata requires the ``sonata_block`` bundle to be configured in your main configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        # ...
        sonata_block:
            default_contexts: [cms]
            blocks:
                # Enable the SonataAdminBundle block
                sonata.admin.block.admin_list:
                    contexts: [admin]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="htp://symfony.com/schema/dic/services">
            <config xmlns="http://sonata-project.org/schema/dic/block">
                <default-context>cms</default-context>

                <block id="sonata.admin.block.admin_list">
                    <context>admin</context>
                </block>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_block', array(
            'default_contexts' => array('cms'),
            'blocks' => array(
                'sonata.admin.block.admin_list' => array(
                    'contexts' => array('admin'),
                ),
            ),
        ));

and it requires the following entries in your routing file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml

        admin:
            resource: '@SonataAdminBundle/Resources/config/routing/sonata_admin.xml'
            prefix: /admin

        _sonata_admin:
            resource: .
            type: sonata_admin
            prefix: /admin

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import
                resource="@SonataAdminBundle/Resources/config/sonata_admin.xml"
                prefix="/admin"
            />

            <import
                resource="."
                type="sonata_admin"
                prefix="/admin"
            />

        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $routing = $loader->import(
            "@SonataAdminBundle/Resources/config/sonata_admin.xml"
        );
        $routing->setPrefix('/admin');
        $collection->addCollection($routing);

        $_sonataAdmin = $loader->import('.', 'sonata_admin');
        $_sonataAdmin->addPrefix('/admin');
        $collection->addCollection($_sonataAdmin);

        return $collection;

and publish your assets (remove ``--symlink`` if you use Windows!):

.. code-block:: bash

    $ php app/console assets:install --symlink web/

Great, now have a look at http://localhost:8000/admin/dashboard

No translations? Uncomment the translator in the configuration file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        # ...
        framework:
            # ...
            translator:      { fallback: %locale% }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/symfony http://symfony.com/schema/dic/symfony/symfony-1.0.xsd">

            <config xmlns="http://symfony.com/schema/dic/symfony">
                <!-- ... -->
                <translator fallback="%locale%" />
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'translator' => array(
                'fallback' => '%locale%',
            ),
        ));

Notice that the adminstration class of the RoutingBundle has been automatically
registered. However, this interface is not required in your application as the routes
are managed by the RoutingAutoBundle and not the administrator. You can disable
the RoutingBundle admin as follows:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            # ...
            dynamic:
                # ...
                persistence:
                    phpcr:
                        # ...
                        use_sonata_admin: false

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <!-- ... -->
                    <persistence>
                        <phpcr use-sonata-admin="false"/>
                    </persistence>
                </dynamic>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', array(
            // ...
            'dynamic' => array(
                'persistence' => array(
                    'phpcr' => array(
                        // ...
                        'use_sonata_admin' => false,
                    ),
                ),
            ),
        ));

.. tip::

    All Sonata Admin aware CMF bundles have such a configuration option and it
    prevents the admin class (or classes) from being registered.

Creating the Admin Classes
~~~~~~~~~~~~~~~~~~~~~~~~~~

Create the following admin classes, first for the ``Page`` document::

    // src/Acme/BasicCmsBundle/Admin/PageAdmin.php
    namespace Acme\BasicCmsBundle\Admin;

    use Sonata\DoctrinePHPCRAdminBundle\Admin\Admin;
    use Sonata\AdminBundle\Datagrid\DatagridMapper;
    use Sonata\AdminBundle\Datagrid\ListMapper;
    use Sonata\AdminBundle\Form\FormMapper;

    class PageAdmin extends Admin
    {
        protected function configureListFields(ListMapper $listMapper)
        {
            $listMapper
                ->addIdentifier('title', 'text')
            ;
        }

        protected function configureFormFields(FormMapper $formMapper)
        {
            $formMapper
                ->with('form.group_general')
                ->add('title', 'text')
                ->add('content', 'textarea')
            ->end();
        }

        public function prePersist($document)
        {
            $parent = $this->getModelManager()->find(null, '/cms/pages');
            $document->setParent($parent);
        }

        protected function configureDatagridFilters(DatagridMapper $datagridMapper)
        {
            $datagridMapper->add('title', 'doctrine_phpcr_string');
        }

        public function getExportFormats()
        {
            return array();
        }
    }

and then for the ``Post`` document - as you have already seen this document is
almost identical to the ``Page`` document, so extend the ``PageAdmin`` class
to avoid code duplication::

    // src/Acme/BasicCmsBundle/Admin/PostAdmin.php
    namespace Acme\BasicCmsBundle\Admin;

    use Sonata\DoctrinePHPCRAdminBundle\Admin\Admin;
    use Sonata\AdminBundle\Datagrid\DatagridMapper;
    use Sonata\AdminBundle\Datagrid\ListMapper;
    use Sonata\AdminBundle\Form\FormMapper;

    class PostAdmin extends PageAdmin
    {
        protected function configureFormFields(FormMapper $formMapper)
        {
            parent::configureFormFields($formMapper);

            $formMapper
                ->with('form.group_general')
                ->add('date', 'date')
            ->end();
        }
    }

.. note::

    In the ``prePersist`` method of the ``PageAdmin`` you specify always a
    fixed path, in the future you may want to modify this behavior to
    enable pages to be structured (for example to have nested menus).

Now you just need to register these classes in the dependency injection
container configuration:

.. configuration-block::

    .. code-block:: yaml

            # src/Acme/BasicCmsBundle/Resources/config/config.yml
            services:
                acme.basic_cms.admin.page:
                    class: Acme\BasicCmsBundle\Admin\PageAdmin
                    arguments:
                        - ''
                        - Acme\BasicCmsBundle\Document\Page
                        - 'SonataAdminBundle:CRUD'
                    tags:
                        - { name: sonata.admin, manager_type: doctrine_phpcr, group: 'Basic CMS', label: Page }
                    calls:
                        - [setRouteBuilder, ['@sonata.admin.route.path_info_slashes']]
                acme.basic_cms.admin.post:
                    class: Acme\BasicCmsBundle\Admin\PostAdmin
                    arguments:
                        - ''
                        - Acme\BasicCmsBundle\Document\Post
                        - 'SonataAdminBundle:CRUD'
                    tags:
                        - { name: sonata.admin, manager_type: doctrine_phpcr, group: 'Basic CMS', label: 'Blog Posts' }
                    calls:
                        - [setRouteBuilder, ['@sonata.admin.route.path_info_slashes']]

    .. code-block:: xml

        <!-- src/Acme/BasicCmsBundle/Resources/config/config.yml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <!-- ... -->
            <services>
                <!-- ... -->
                <service id="acme.basic_cms.admin.page"
                    class="Acme\BasicCmsBundle\Admin\PageAdmin">

                    <call method="setRouteBuilder">
                        <argument type="service" id="sonata.admin.route.path_info_slashes" />
                    </call>

                    <tag
                        name="sonata.admin"
                        manager_type="doctrine_phpcr"
                        group="Basic CMS"
                        label="Page"
                    />
                    <argument/>
                    <argument>Acme\BasicCmsBundle\Document\Page</argument>
                    <argument>SonataAdminBundle:CRUD</argument>
                </service>

                <service id="acme.basic_cms.admin.post"
                    class="Acme\BasicCmsBundle\Admin\PostAdmin">

                    <call method="setRouteBuilder">
                        <argument type="service" id="sonata.admin.route.path_info_slashes" />
                    </call>

                    <tag
                        name="sonata.admin"
                        manager_type="doctrine_phpcr"
                        group="Basic CMS"
                        label="Blog Posts"
                    />
                    <argument/>
                    <argument>Acme\BasicCmsBundle\Document\Post</argument>
                    <argument>SonataAdminBundle:CRUD</argument>
                </service>
            </services>
        </container>

    .. code-block:: php

            // src/Acme/BasicCmsBundle/Resources/config/config.php
            use Symfony\Component\DependencyInjection\Reference;
            // ...

            $container->register('acme.basic_cms.admin.page', 'Acme\BasicCmsBundle\Admin\PageAdmin')
              ->addArgument('')
              ->addArgument('Acme\BasicCmsBundle\Document\Page')
              ->addArgument('SonataAdminBundle:CRUD')
              ->addTag('sonata.admin', array(
                  'manager_type' => 'doctrine_phpcr',
                  'group' => 'Basic CMS',
                  'label' => 'Page'
              )
              ->addMethodCall('setRouteBuilder', array(
                  new Reference('sonata.admin.route.path_info_slashes'),
              ))
            ;
            $container->register('acme.basic_cms.admin.post', 'Acme\BasicCmsBundle\Admin\PostAdmin')
              ->addArgument('')
              ->addArgument('Acme\BasicCmsBundle\Document\Post')
              ->addArgument('SonataAdminBundle:CRUD')
              ->addTag('sonata.admin', array(
                   'manager_type' => 'doctrine_phpcr',
                   'group' => 'Basic CMS',
                   'label' => 'Blog Posts'
              )
              ->addMethodCall('setRouteBuilder', array(
                  new Reference('sonata.admin.route.path_info_slashes'),
              ))
            ;

.. note::

    In the XML version of the above configuration you specify ``manager_type``
    (with an underscore). This should be `manager-type` (with a hypen) and
    will be fixed in Symfony version 2.4.

Check it out at http://localhost:8000/admin/dashboard

.. image:: ../../_images/cookbook/basic-cms-sonata-admin.png

.. _SonataDoctrinePHPCRAdminBundle: http://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
