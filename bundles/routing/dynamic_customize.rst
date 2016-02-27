.. _bundle-routing-customize:

Customizing the Dynamic Router
==============================

The ``DynamicRouter`` is built to be customized. Both route matching and URL
generation services can be injected, and the provided route matcher and
URL generator classes are built to be further customizable.

This chapter describes the most common customization. If you want to go
further, you will need to read the
:doc:`component documentation <../../components/routing/introduction>`
and look into the source code.

If the ``DynamicRouter`` does not fit your needs at all, you have the option
of writing your own routers to hook into the ``ChainRouter``.

.. index:: Route Enhancer

Writing your own Route Enhancers
--------------------------------

You can add your own :ref:`RouteEnhancerInterface <bundles-routing-dynamic_router-enhancer>`
implementations if you have a case not handled by the
:ref:`provided enhancers <component-routing-enhancers>`.

.. code-block:: php

    // src/AppBundle/Routing/Enhancer/SimpleEnhancer.php
    namespace AppBundle\Routing\Enhancer;

    use Symfony\Cmf\Component\Routing\Enhancer\RouteEnhancerInterface;
    use Symfony\Component\HttpFoundation\Request;

    class SimpleEnhancer implements RouteEnhancerInterface
    {
        public function enhance(array $defaults, Request $request)
        {
            // ... customize the $defaults array

            return $defaults;
        }
    }

Simply define services for your enhancers and tag them with
``dynamic_router_route_enhancer`` to have them added to the routing. You can
specify an optional ``priority`` parameter on the tag to control the order in
which enhancers are executed. The higher the priority, the earlier the enhancer
is executed.

.. configuration-block::

    .. code-block:: yaml

        services:
            app.routing.simple_enhancer:
                class: AppBundle\Routing\Enhancer\SimpleEnhancer
                tags:
                    -  { name: dynamic_router_route_enhancer, priority: 10 }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="app.routing.simple_enhancer" class="AppBundle\Routing\Enhancer\SimpleEnhancer">
                    <tag name="dynamic_router_route_enhancer" priority="10" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('AppBundle\Routing\Enhancer\SimpleEnhancer');
        $definition->addTag('dynamic_router_route_enhancer', array(
            'priority' => 10,
        ));

        $container->setDefinition('app.routing.simple_enhancer', $definition);

.. index:: Route Provider

.. _bundle-routing-custom_provider:

Using a Custom Route Provider
-----------------------------

The Dynamic Router allows you to customize the Route Provider (i.e. the class
responsible for retrieving routes from the database) and, by extension, the
Route objects.

Creating the Route Provider
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The route provider must implement the ``RouteProviderInterface``. The
following class provides a simple solution using an ODM Repository.

.. code-block:: php

    // src/Acme/DemoBundle/Repository/RouteProvider.php
    namespace Acme\DemoBundle\Repository;

    use Doctrine\ODM\PHPCR\DocumentRepository;
    use Symfony\Cmf\Component\Routing\RouteProviderInterface;
    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Route as SymfonyRoute;

    class RouteProvider extends DocumentRepository implements RouteProviderInterface
    {
        /**
         * This method is used to find routes matching the given URL.
         */
        public function findManyByUrl($url)
        {
            // for simplicity we retrieve one route
            $document = $this->findOneBy(array(
                'url' => $url,
            ));

            $pattern = $document->getUrl(); // e.g. "/this/is/a/url"

            $collection = new RouteCollection();

            // create a new Route and set our document as
            // a default (so that we can retrieve it from the request)
            $route = new SymfonyRoute($pattern, array(
                'document' => $document,
            ));

            // add the route to the RouteCollection using
            // a unique ID as the key.
            $collection->add('my_route_'.uniqid(), $route);

            return $collection;
        }

        /**
         * This method is used to generate URLs, e.g. {{ path('foobar') }}.
         */
        public function getRouteByName($name, $params = array())
        {
            $document = $this->findOneBy(array(
                'name' => $name,
            ));

            if ($route) {
                $route = new SymfonyRoute($route->getPattern(), array(
                    'document' => $document,
                ));
            }

            return $route;
        }
    }

.. tip::

    As you may have noticed we return a ``RouteCollection`` object - why not
    return a single ``Route``? The Dynamic Router allows us to return many
    *candidate* routes, in other words, routes that *might* match the incoming
    URL. This is important to enable the possibility of matching *dynamic*
    routes, ``/page/{page_id}/edit`` for example. In our example we match the
    given URL exactly and only ever return a single ``Route``.

Replacing the Default CMF Route Provider
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To replace the default ``RouteProvider``, it is necessary to modify your
configuration as follows:

.. configuration-block::

   .. code-block:: yaml

       # app/config/config.yml
       cmf_routing:
           dynamic:
               enabled: true
               route_provider_service_id: acme_demo.provider.endpoint

   .. code-block:: xml

       <!-- app/config/config.xml -->
       <?xml version="1.0" encoding="UTF-8" ?>
       <container xmlns="http://symfony.com/schema/dic/services">
           <config xmlns="http://cmf.symfony.com/schema/dic/routing">
               <dynamic
                   enabled="true"
                   route-provider-service-id="acme_demo.provider.endpoint"
               />
           </config>
       </container>

   .. code-block:: php

       // app/config/config.php
       $container->loadFromExtension('cmf_routing', array(
           'dynamic' => array(
              'enabled'                   => true,
              'route_provider_service_id' => 'acme_demo.provider.endpoint',
           ),
       ));

Where ``acme_demo.provider.endpoint`` is the service ID of your route
provider.  See `Creating and configuring services in the container`_ for
information on creating custom services.

.. _`Creating and configuring services in the container`: http://symfony.com/doc/current/book/service_container.html#creating-configuring-services-in-the-container/
.. _`PHPCR-ODM`: http://www.doctrine-project.org/projects/phpcr-odm.html
