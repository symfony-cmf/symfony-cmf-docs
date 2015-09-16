.. index::
    single: The Router; Quick Tour

The Router
==========

Welcome at the third part of the Quick Tour. You seem to have fallen in love
with the CMF, getting this far! And that's a good thing, as you will learn
about the backbone of the CMF in this chapter: The Router.

The Backbone of the CMF
-----------------------

As already said, the router is the backbone. To understand this, you have a
good view of what a CMS tries to do. In a normal Symfony application, a route
refers to a controller which can handle a specific entity. Another route
refers to another controller which can handle another entity. This way, a
route is tied to a controller. In fact, using the Symfony core you are also
limited at this.

But if you look at the base of a CMS, it only needs to handle 1 type of
entity: The Content. So most of the routes don't have to be tied to a
controller anymore, as only one controller is needed. The Route has to be tied
to a specific Content object, which - on its side - can reference a specific
template and controller.

Other parts of the CMF are also related to the Router. To give 2 examples: The
menu is created by generating specific routes using the Router and the blocks
are displayed to specific routes (as they are related to a template).

Loading Routes from the PHPCR tree
----------------------------------

In the first chapter, you have already learned that routes are loaded from the
database using a special ``DynamicRouter``. This way, not all routes need to
be loaded each request.

Matching routes from a PHPCR is really simple. If you remember the previous
chapter, you know that you can get the ``quick_tour`` page from PHPCR using
``/cms/simple/quick_tour``. The URL to get this page is ``quick_tour``. Some
other examples:

.. code-block:: text

    /cms
        /simple
            /about       # /about Route
            /contact     # /contact Route
                /team    # /contact/team Route
                /docs    # /docs Route

OK, you got it? The only thing the Router has to do is prefix the route with a
specific path prefix and load that document. In the case of the SimpleCmsBundle,
all routes are prefixed with ``/cms/simple``.

You see that a route like ``/contact/team``, which consist of 2 "path units",
has 2 documents in the PHPCR tree: ``contact`` and ``team``.

Chaining multiple Routers
-------------------------

You may need to have several prefixes or several routes. For instance, you may
want to use both the ``DynamicRouter`` for the page routes, but also the
static routing files from Symfony for your custom logic. To be able to do that,
the CMF provides a ``ChainRouter``. This router chains over multiple router
and stops whenever a router matches.

By default, the ``ChainRouter`` overrides the Symfony router and only has the
core router in its chain. You can add more routers to the chain in the
configuration or by tagging the router services. For instance, the router used
by the SimpleCmsBundle is a service registered by that bundle and tagged with
``cmf_routing.router``.

Creating a new Route
--------------------

Now you know the basics of routing, you can add a new route to the tree. In
the configuration file, configure a new chain router so that you can put your
new routes in ``/cms/routes``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        # ...
        cmf_routing:
            chain:
                routers_by_id:
                    # the standard DynamicRouter
                    cmf_routing.dynamic_router: 200

                    # the core symfony router
                    router.default: 100
            dynamic:
                persistence:
                    phpcr:
                        route_basepath: /cms/routes

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <!-- ... -->

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <chain>
                    <!-- the standard DynamicRouter -->
                    <router-by-id id="cmf_routing.dynamic_router">200</router-by-id>

                    <!-- the core symfony router -->
                    <router-by-id id="router.default">100</router-by-id>
                </chain>

                <dynamic>
                    <persistence>
                        <phpcr route-basepath="/cms/routes" />
                    </persistence>
                </dynamic>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', array(
            'chain' => array(
                'routers_by_id' => array(
                    // the standard DynamicRouter
                    'cmf_routing.dynamic_router' => 200,

                    // the core symfony router
                    'router.default' => 100,
                ),
            ),
            'dynamic' => array(
                'persistence' => array(
                    'phpcr' => array(
                        'route_basepath' => '/cms/routes',
                    ),
                ),
            ),
        ));

Now you can add a new ``Route`` to the tree using Doctrine::

    // src/Acme/DemoBundle/DataFixtures/PHPCR/LoadRoutingData.php
    namespace Acme\DemoBundle\DataFixtures\PHPCR;

    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\Common\DataFixtures\FixtureInterface;

    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;

    class LoadRoutingData implements FixtureInterface
    {
        public function load(ObjectManager $documentManager)
        {
            $routesRoot = $documentManager->find(null, '/cms/routes');

            $route = new Route();
            // set $routesRoot as the parent and 'new-route' as the node name,
            // this is equal to:
            // $route->setName('new-route');
            // $route->setParentDocument($routesRoot);
            $route->setPosition($routesRoot, 'new-route');

            $page = $documentManager->find(null, '/cms/routes/quick_tour');
            $route->setContent($page);

            $documentManager->persist($route); // put $route in the queue
            $documentManager->flush(); // save it
        }
    }

This creates a new node called ``/cms/routes/new-route``, which will display
our ``quick_tour`` page when you go to ``/new-route``.

.. tip::

    When doing this in a real app, you may want to use a ``RedirectRoute``
    instead.

.. TODO write something about templates_by_class, etc.

Final Thoughts
--------------

Now you reached the end of this article, you can say you really know the
basics of the Symfony CMF. First, you have learned about the Request flow and
quickly learned each new step in this process. After that, you have learned
more about the default storage layer and the routing system.

The Routing system is created together with some developers from Drupal8. In
fact, Drupal 8 uses the Routing component of the Symfony CMF. The Symfony CMF
also uses some 3rd party bundles from others and integrated them into PHPCR.
In :doc:`the next chapter <the_third_party_bundles>` you'll learn more about
those bundles and other projects the Symfony CMF is helping.
