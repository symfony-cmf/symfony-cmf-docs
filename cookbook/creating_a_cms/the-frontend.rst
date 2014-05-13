Creating a Menu
---------------

In this section you will modify your application so that ``Page``
documents act as menu nodes. The root page document can then be rendered
using the Twig helper of the `KnpMenuBundle`_.

Installation
............

Ensure that the following package is installed:

.. code-block:: javascript

    {
        ...
        require: {
            ...
            "symfony-cmf/menu-bundle": "1.1.*"
        },
        ...
    }

Add the CMF `MenuBundle`_ and its dependency, `CoreBundle`_, to your kernel::

    // app/AppKernel.php
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Knp\Bundle\MenuBundle\KnpMenuBundle(),
                new Symfony\Cmf\Bundle\CoreBundle\CmfCoreBundle(),
                new Symfony\Cmf\Bundle\MenuBundle\CmfMenuBundle(),
            );

            // ...
        }
    }

Modify the Page Document
........................

The menu document has to implement the ``Knp\Menu\NodeInterface``
provided by the KnpMenuBundle::

    // src/Acme/BasicCmsBundle/Document/Page.php
    namespace Acme\BasicCmsBundle\Document;

    // ...
    use Knp\Menu\NodeInterface;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

    class Page implements RouteReferrersReadInterface, NodeInterface
    {
        // ...

        /**
         * @PHPCR\Children()
         */
        protected $children;

        public function getName()
        {
            return $this->title;
        }

        public function getChildren()
        {
            return $this->children;
        }

        public function getOptions()
        {
            return array(
                'label' => $this->title,
                'content' => $this,

                'attributes'         => array(),
                'childrenAttributes' => array(),
                'displayChildren'    => true,
                'linkAttributes'     => array(),
                'labelAttributes'    => array(),
            );
        }
    }

.. caution::

    In a typical CMF application, there are two ``NodeInterface`` which
    have nothing to do with each other. The interface we use here is from
    KnpMenuBundle and describes menu tree nodes. The other interface is
    from the PHP content repository and describes content repository
    tree nodes.

Menus are hierarchical, PHPCR-ODM is also hierarchical and so lends itself
well to this use case.

Here you add an additional mapping, ``@Children``, which will cause PHPCR-ODM
to populate the annotated property instance ``$children`` with the child
documents of this document.

The options are the options used by KnpMenu system when rendering the menu.
The menu URL is inferred from the ``content`` option (note that you added the
``RouteReferrersReadInterface`` to ``Page`` earlier).

The attributes apply to the HTML elements. See the `KnpMenu`_ documentation
for more information.

Modify the Data Fixtures
........................

The menu system expects to be able to find a root item which contains the
first level of child items. Modify your fixtures to declare a root element
to which you will add the existing ``Home`` page and an additional ``About`` page::

    // src/Acme/BasicCmsBundle/DataFixtures/Phpcr/LoadPageData.php

    // ...
    class LoadPageData implements FixtureInterface
    {
        public function load(ObjectManager $dm)
        {
            // ...
            $rootPage = new Page();
            $rootPage->setTitle('main');
            $rootPage->setParentDocument($parent);
            $dm->persist($rootPage);

            $page = new Page();
            $page->setTitle('Home');
            $page->setParentDocument($rootPage);
            $page->setContent(<<<HERE
    Welcome to the homepage of this really basic CMS.
    HERE
            );
            $dm->persist($page);

            $page = new Page();
            $page->setTitle('About');
            $page->setParentDocument($rootPage);
            $page->setContent(<<<HERE
    This page explains what its all about.
    HERE
            );
            $dm->persist($page);

            $dm->flush();
        }
    }

Load the fixtures again:

.. code-block:: bash

    $ php app/console doctrine:phpcr:fixtures:load

Register the Menu Provider
..........................

Now you can register the ``PhpcrMenuProvider`` from the menu bundle in the service container
configuration:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BasicCmsBundle/Resources/config/config.yml
        services:
            acme.basic_cms.menu_provider:
                class: Symfony\Cmf\Bundle\MenuBundle\Provider\PhpcrMenuProvider
                arguments:
                    - '@cmf_menu.factory'
                    - '@doctrine_phpcr'
                    - /cms/pages
                calls:
                    - [setRequest, ["@?request="]]
                tags:
                    - { name: knp_menu.provider }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_demo="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <!-- ... -->
            <services>
                <!-- ... -->
                <service
                    id="acme.basic_cms.menu_provider"
                    class="Symfony\Cmf\Bundle\MenuBundle\Provider\PhpcrMenuProvider">
                    <argument type="service" id="cmf_menu.factory"/>
                    <argument type="service" id="doctrine_phpcr"/>
                    <argument>/cms/pages</argument>
                    <call method="setRequest">
                        <argument
                            type="service"
                            id="request"
                            on-invalid="null"
                            strict="false"
                        />
                    </call>
                    <tag name="knp_menu.provider" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/BasicCmsBundle/Resources/config/config.php
        use Symfony\Component\DependencyInjection\Reference;
        // ...

        $container
            ->register(
                'acme.basic_cms.menu_provider',
                'Symfony\Cmf\Bundle\MenuBundle\Provider\PhpcrMenuProvider'
            )
            ->addArgument(new Reference('cmf_menu.factory'))
            ->addArgument(new Reference('doctrine_phpcr'))
            ->addArgument('/cms/pages')
            ->addMethodCall('setRequest', array(
                new Reference(
                    'request',
                    ContainerInterface::NULL_ON_INVALID_REFERENCE,
                    false
                )
            ))
            ->addTag('knp_menu.provider')
        ;

and enable the Twig rendering functionality of the KnpMenu bundle:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        knp_menu:
            twig: true

    .. code-block:: xml

        <!-- app/config/config.yml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://example.org/schema/dic/knp_menu">
                <twig>true</twig>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('knp_menu', array(
            'twig' => true,
        ));

and finally you can render the menu!

.. configuration-block::

    .. code-block:: jinja

        {# src/Acme/BasicCmsBundle/Resources/views/Default/page.html.twig #}

        {# ... #}
        {{ knp_menu_render('main') }}

    .. code-block:: html+php

        <!-- src/Acme/BasicCmsBundle/Resources/views/Default/page.html.php -->

        <!-- ... -->
        <?php echo $view['knp_menu']->render('main') ?>

Note that ``main`` refers to the name of the root page you added in the data
fixtures.

.. _`knpmenubundle`: https://github.com/KnpLabs/KnpMenuBundle
.. _`knpmenu`: https://github.com/KnpLabs/KnpMenu
.. _`MenuBundle`: https://github.com/symfony-cmf/MenuBundle
.. _`CoreBundle`: https://github.com/symfony-cmf/CoreBundle
