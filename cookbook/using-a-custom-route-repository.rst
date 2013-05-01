Using a custom route repository with Dynamic Router
===================================================

The Dynamic Router allows you to customize the route Provider (i.e. the class
responsible for retrieving routes from the database), and by extension, the
Route objects.

Creating the route provider
-----------------------------

The route provider must implement the `RouteProviderInterface` The
following class provides a simple solution using an ODM Repository.

.. code-block:: php

    <?php

    namespace MyVendor\Bundle\MyBundle\Repository;
    use Doctrine\ODM\PHPCR\DocumentRepository;
    use Symfony\Cmf\Component\Routing\RouteProviderInterface;
    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Route as SymfonyRoute;

    class RouteProvider extends DocumentRepository implements RouteProviderInterface

    {
        // this method is used to find routes matching the given URL
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

        // this method is used to generate URLs, e.g. {{ path('foobar') }}
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

    As you may have noticed we return a `RouteCollection` object - why not return
    a single `Route`? The Dynamic Router allows us to return many *candidate* routes,
    in other words, routes that *might* match the incoming URL. This is important to
    enable the possibility of matching *dynamic* routes, `/page/{page_id}/edit` for example.
    In our example we match the given URL exactly and only ever return a single `Route`.

Replacing the default CMF route provider
------------------------------------

To replace the default `RouteProvider` it is necessary to modify your configuration
as follows:

.. configuration-block::

   .. code-block:: yaml

       # app/config/config.yml
       symfony_cmf_routing:
           dynamic:
               enabled: true
               route_provider_service_id: my_bundle.provider.endpoint

Where `my_bundle.provider.endpoint` is the service ID of your route provider.
See `Creating and configuring services in the container <http://symfony.com/doc/current/book/service_container.html#creating-configuring-services-in-the-container/>`_
for information on creating custom services.
