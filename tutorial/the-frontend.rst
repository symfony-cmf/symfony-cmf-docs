Creating a Menu
===============

In this section you will modify your application so that ``Page``
documents act as menu nodes. The root page document can then be rendered
using the Twig helper of the `KnpMenuBundle`_.

Installation
------------

Ensure that you installed the ``symfony-cmf/menu-bundle`` package as detailed
in the :ref:`gettingstarted_installadditionbundles` section.

Add the CMF :doc:`MenuBundle <../bundles/menu/introduction>` and its
dependency, :doc:`CoreBundle <../bundles/core/introduction>`, to your kernel::

    // app/AppKernel.php
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Cmf\Bundle\CoreBundle\CmfCoreBundle(),
                new Symfony\Cmf\Bundle\MenuBundle\CmfMenuBundle(),
            );

            // ...
        }
    }

.. note::

    The KnpMenuBundle is also required but was already included in the
    :doc:`sonata-admin` chapter. If you skipped that chapter be sure to add
    this bundle now.

Modify the Page Document
------------------------

The menu document has to implement the ``Knp\Menu\NodeInterface``
provided by the KnpMenuBundle. Modify the Page document so that it
implements this interface::

    // src/AppBundle/Document/Page.php
    namespace AppBundle\Document;

    // ...
    use Knp\Menu\NodeInterface;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

    class Page implements RouteReferrersReadInterface, NodeInterface

Now add the following to the document to fulfill the contract::

    // src/AppBundle/Document/Page.php

    // ...
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
------------------------

The menu system expects to be able to find a root item which contains the
first level of child items. Modify your fixtures to declare a root element
to which you will add the existing ``Home`` page and an additional ``About`` page::

    // src/AppBundle/DataFixtures/PHPCR/LoadPageData.php
    namespace AppBundle\DataFixtures\PHPCR;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\ODM\PHPCR\DocumentManager;
    use AppBundle\Document\Page;

    class LoadPageData implements FixtureInterface
    {
        public function load(ObjectManager $dm)
        {
            if (!$dm instanceof DocumentManager) {
                $class = get_class($dm);
                throw new \RuntimeException("Fixture requires a PHPCR ODM DocumentManager instance, instance of '$class' given.");
            }

            $parent = $dm->find(null, '/cms/pages');

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
--------------------------

Now you can register the ``PhpcrMenuProvider`` from the menu bundle in the service container
configuration:

.. configuration-block::

    .. code-block:: yaml

        # src/AppBundle/Resources/config/services.yml
        services:
            app.menu_provider:
                class: Symfony\Cmf\Bundle\MenuBundle\Provider\PhpcrMenuProvider
                arguments:
                    - '@cmf_menu.loader.node'
                    - '@doctrine_phpcr'
                    - /cms/pages
                tags:
                    - { name: knp_menu.provider }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:app="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <!-- ... -->
            <services>
                <!-- ... -->
                <service
                    id="app.menu_provider"
                    class="Symfony\Cmf\Bundle\MenuBundle\Provider\PhpcrMenuProvider"
                >
                    <argument type="service" id="cmf_menu.loader.node"/>
                    <argument type="service" id="doctrine_phpcr"/>
                    <argument>/cms/pages</argument>

                    <tag name="knp_menu.provider" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/AppBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Reference;
        // ...

        $container
            ->register(
                'app.menu_provider',
                'Symfony\Cmf\Bundle\MenuBundle\Provider\PhpcrMenuProvider'
            )
            ->addArgument(new Reference('cmf_menu.loader.node'))
            ->addArgument(new Reference('doctrine_phpcr'))
            ->addArgument('/cms/pages')
            ->addTag('knp_menu.provider')
        ;

.. versionadded:: 2.0
    The first argument of the ``PhpcrMenuProvider`` class was changed in CmfMenuBundle 2.0.
    You had to inject the ``cmf_menu.factory`` service prior to version 2.0.

and enable the Twig rendering functionality of the KnpMenuBundle:

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

        {# src/AppBundle/Resources/views/Default/page.html.twig #}

        {# ... #}
        {{ knp_menu_render('main') }}

    .. code-block:: html+php

        <!-- src/AppBundle/Resources/views/Default/page.html.php -->

        <!-- ... -->
        <?php echo $view['knp_menu']->render('main') ?>

Note that ``main`` refers to the name of the root page you added in the data
fixtures.

.. _`knpmenubundle`: https://github.com/KnpLabs/KnpMenuBundle
.. _`knpmenu`: https://github.com/KnpLabs/KnpMenu
.. _`MenuBundle`: https://github.com/symfony-cmf/menu-bundle
.. _`CoreBundle`: https://github.com/symfony-cmf/core-bundle
