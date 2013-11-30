Part 4 - The Frontend
---------------------

Mapping Content to Controllers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Go to the URL http://localhost:8000/page/home in your browser - this should be
your page, but it says that it cannot find a controller. In other words it has
found the *page referencing route* for your page but Symfony does not know what
to do with it.

You can map a default controller for all instances of ``Page``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                # ...
                controllers_by_class:
                    Acme\BasicCmsBundle\Document\Page: Acme\BasicCmsBundle\Controller\DefaultController::pageAction

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic generic-controller="cmf_content.controller:indexAction">
                    <!-- ... -->
                    <controllers-by-class
                        class="Acme\BasicCmsBundle\Document\Page"
                    >
                        Acme\BasicCmsBundle\Controller\DefaultController::pageAction
                    </controllers-by-class>
                </dynamic>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                // ...
                'controllers_by_class' => array(
                    'Acme\BasicCmsBundle\Document\Page' => 'Acme\BasicCmsBundle\Controller\DefaultController::pageAction',
                ),
            ),
        ));

This will cause requests to be forwarded to this controller when the route
which matches the incoming request is provided by the dynamic router **and**
the content document that that route references is of class
``Acme\BasicCmsBundle\Document\Page``

Now create the action in the default controller - you can pass the ``Page``
object and all the ``Posts`` to the view::

    // src/Acme/BasicCmsBundle/Controller/DefaultController.php
    //..
    class DefaultController extends Controller
    {
        // ...

        /**
         * @Template()
         */
        public function pageAction($contentDocument)
        {
            $dm = $this->get('doctrine_phpcr')->getManager();
            $posts = $dm->getRepository('Acme\BasicCmsBundle\Document\Post')->findAll();

            return array(
                'page' => $contentDocument,
                'posts' => $posts,
            );
        }
    }

The ``Page`` object is passed automatically as ``$contentDocument``.

Add a corresponding twig template (note that this works because you use the
``@Template`` annotation):

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BasicCmsBundle/Resources/views/Default/page.html.twig #}
        <h1>{{ page.title }}</h1>
        <p>{{ page.content|raw }}</p>
        <h2>Our Blog Posts</h2>
        <ul>
            {% for post in posts %}
                <li><a href="{{ path(post) }}">{{ post.title }}</a></li>
            {% endfor %}
        </ul>

    .. code-block:: html+php

        <!-- src/Acme/BasicCmsBundle/Resources/views/Default/page.html.twig -->
        <h1><?php echo $page->getTitle() ?></h1>
        <p><?php echo $page->content ?></p>
        <h2>Our Blog Posts</h2>
        <ul>
            <?php foreach($posts as $post) : ?>
                <li>
                    <a href="<?php echo $view['router']->generate($post) ?>">
                        <?php echo $post->getTitle() ?>
                    </a>
                </li>
            <?php endforeach ?>
        </ul>

Now have another look at: http://localhost:8000/page/home

Notice what is happening with the post object and the ``path`` function  - you
pass the ``Post`` object and the ``path`` function will pass the object to the
router and because it implements the ``RouteReferrersReadInterface`` the
``DynamicRouter`` will be able to generate the URL for the post.

Click on a ``Post`` and you will have the same error that you had before when
viewing the page at ``/home`` and you can resolve it in the same way.

Creating a Menu
~~~~~~~~~~~~~~~

In this section you will modify your application so that ``Page``
documents act as menu nodes. The root page document can then be rendered
using the twig helper of the `KnpMenuBundle`_.

Modify the Page Document
........................

The menu document has to implement the ``NodeInterface`` provided by the
KnpMenuBundle::

    // src/Acme/BasicCmsBundle/Document/Page.php
    namespace Acme\BasicCmsBundle\Document;

    // ...
    use Knp\Menu\NodeInterface;

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

                'attributes' => array(),
                'childrenAttributes' => array(),
                'displayChildren' => true,
                'linkAttributes' => array(),
                'labelAttributes' => array(),
            );
        }
    }

.. note::

    Don't forget to add the ``Knp\Menu\NodeInterface`` use statement!

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
            $rootPage->setParent($parent);
            $dm->persist($rootPage);

            $page = new Page();
            $page->setTitle('Home');
            $page->setParent($rootPage);
            $page->setContent(<<<HERE
    Welcome to the homepage of this really basic CMS.
    HERE
            );
            $dm->persist($page);

            $page = new Page();
            $page->setTitle('About');
            $page->setParent($rootPage);
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

First you will need to add the CMF `MenuBundle`_ and its dependency, `CoreBundle`_, to your
applications kernel::

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

Now you can register the PhpcrMenuProvider from the menu bundle in the service container
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
                 tags:
                     - { name: knp_menu.provider }
                     - { name: cmf_request_aware }

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
                    <tag name="knp_menu.provider" />
                    <tag name="cmf_request_aware"/>
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
            ->addTag('knp_menu.provider')
            ->addTag('cmf_request_aware')
        ;

and enable the twig rendering functionality of the KnpMenu bundle:

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

and finally lets render the menu!

.. configuration-block::

    .. code-block:: jinja
        
        {# src/Acme/BasicCmsBundle/Resources/views/Default/page.html.twig #}

        {# ... #}
        {{ knp_menu_render('main') }}

    .. code-block:: html+php

        <!-- src/Acme/BasicCmsBundle/Resources/views/Default/page.html.php -->
        
        <?php echo $view['knp_menu']->render('main') ?>

Note that ``main`` refers to the name of the root page you added in the data
fixtures.

.. _`knpmenubundle`: https://github.com/KnpLabs/KnpMenuBundle
.. _`knpmenu`: https://github.com/KnpLabs/KnpMenu
.. _`MenuBundle`: https://github.com/symfony-cmf/MenuBundle
.. _`CoreBundle`: https://github.com/symfony-cmf/CoreBundle
