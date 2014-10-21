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
:ref:`provided enhancers <component-routing-enhancers>`. Simply define services
for your enhancers and tag them with ``dynamic_router_route_enhancer`` to have
them added to the routing. You can specify an optional ``priority`` parameter
on the tag to control the order in which enhancers are executed. The higher the
priority, the earlier the enhancer is executed.

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

    <?php
    // src/Acme/DemoBundle/Repository/RouteProvider.php
    namespace Acme\DemoBundle\Repository;

    use Doctrine\Bundle\PHPCRBundle\ManagerRegistry;
    use Doctrine\ODM\PHPCR\DocumentManager;
    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;
    use Symfony\Cmf\Bundle\RoutingBundle\Routing\DynamicRouter;
    use Symfony\Cmf\Component\Routing\RouteProviderInterface;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\Routing\Route as SymfonyRoute;
    use Symfony\Component\Routing\RouteCollection;

    class RouteProvider implements RouteProviderInterface
    {
        /**
         * @var ManagerRegistry
         */
        private $managerRegistry;

        /**
         * To get the Document Manager out of the registry.
         *
         * @return DocumentManager
         */
        private function getDocumentManager()
        {
            return $this->managerRegistry->getManager();
        }

        public function __construct(ManagerRegistry $managerRegistry)
        {
            $this->managerRegistry = $managerRegistry;
        }

        public function getRouteCollectionForRequest(Request $request)
        {
            $path = $request->getPathInfo();
            $candidates = $this->getCandidates($path);

            $routeCandidates = $this->getDocumentManager()->findMany(null, $candidates);
            $routeCollection = new RouteCollection();

            $count = 0;
            foreach ($routeCandidates as $candidate) {
                $count++;
                if ($candidate instanceof Route) {
                    $defaults = $candidate->getDefaults();
                    $defaults[DynamicRouter::CONTENT_KEY] = $candidate->getContent();
                    $routeCollection->add(
                        'my_route_'.$count,
                        new SymfonyRoute($candidate->getPath(), $defaults)
                    );
                }
            }

            return $routeCollection;
        }

        /**
         * {@inheritDoc}
         */
        public function getRouteByName($name, $parameters = array())
        {
            /** @var Route $route */
            $route = $this->getDocumentManager()->find(null, $name);

            if ($route) {
                $defaults = $route->getDefaults();
                $defaults[DynamicRouter::CONTENT_KEY] = $route->getContent();
                $route = new SymfonyRoute($route->getPath(), $defaults);
            }

            return $route;
        }

        public function getRoutesByNames($names, $parameters = array())
        {

        }

        /**
         * Method to to create the paths to look for the current route.
         *
         * @param string
         */
        private function getCandidates($path)
        {
            // add your route base paths
            $prefixes = array(
                '/cms/routes',
            );

            $result = array();
            foreach ($prefixes as $prefix) {
                $result[] = $prefix.$path;
            }

            return $result;
        }
    }

.. tip::

    The ``RouteProviderInteface`` will force to implement the shown above.
    As you may have noticed we return in ``getRouteCollectionForRequest``
    and ``getRoutesByNames`` a ``RouteCollection`` object - why not
    return a single ``Route``? The Dynamic Router allows us to return many
    *candidate* routes, in other words, routes that *might* match the incoming
    URL. This is important to enable the possibility of matching *dynamic*
    routes, ``/page/{page_id}/edit`` for example.

    If you set some defaults for your route (template, controller, etc.), they will
    be added as options to the Symfony route. As you may have noticed the example
    added the mapped document with a specific key ``DynamicRouter::CONTENT_KEY``
    to the defaults array. By doing this you will find the current document in
    the requests parameter bag in
    ``$parameterBag[...]['my_route_1'][DynamicRouter::CONTENT_KEY]``
    to manipulate it in listeners for example. But most important part is:
    The document will be injected to your action by adding a parameter with
    that name.

Replacing the Default CMF Route Provider
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To replace the default ``RouteProvider``, it is necessary to modify your
configuration as follows:

.. configuration-block::

   .. code-block:: yaml

       // app/config/config.yml
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
information on creating custom services. In our example the service definition
will look like this:

.. configuration-block::

   .. code-block:: yaml

        parameters:
            acme_demo.provider.endpoint.class: Acme\DemoBundle\Repository\RouteProvider

        services:
            acme_demo.provider.endpoint:
                class: "%acme_demo.provider.endpoint.class%"
                arguments: ["@doctrine_phpcr"]

   .. code-block:: xml

       <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <parameters>
                <parameter key="acme_demo.provider.endpoint.class">Acme\Repository\RouteProvider</parameter>
            </parameters>

            <services>
                <service id="acme_demo.provider.endpoint" class="%acme_demo.provider.endpoint.class%">
                    <argument type="service" id="doctrine_phpcr"/>
                </service>
            </services>

        </container>


   .. code-block:: php

        $container->setParameter(
            'acme_demo.provider.endpoint.class',
            'Acme\DemoBundle\Repository\RouteProvider'
        );

        $container
            ->register('acme_demo.provider.endpoint', '%acme_demo.provider.endpoint.class%')
            ->addArgument(new Reference('doctrine_phpcr'))
        ;

As you can see the ``DocumentRegistry`` of Doctrine PHPCR-ODM is injected.
This will provide the document manger the provider needs to query
the persistence implementation. As the RouteProvider is a Symfony solution
you can inject what you want - you should somehow return a ``Route`` or
``RouteCollection`` in your providers methods - it is up to you.

.. _`Creating and configuring services in the container`: http://symfony.com/doc/current/book/service_container.html#creating-configuring-services-in-the-container/
.. _`PHPCR-ODM`: http://www.doctrine-project.org/projects/phpcr-odm.html
