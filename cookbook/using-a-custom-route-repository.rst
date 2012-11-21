Using a custom route repository with Dynmaic Router
===================================================

The Dynamic Router allows you to customize the route Repository (i.e. the class 
responsible for retrieving routes from the database), and by extension, the 
Route objects.

Creating the route repository
-----------------------------

The route repository must implement the `RouteRepositoryInterface` and in 
addition should return instances of the Symfony Route class. The 
following class provides a simple solution using an ODM Repository. For the
purpose of example we create the Symfony Route object and map the pattern
to it directly.

Why return multiple routes? .. @todo explain that here.

.. code-block:: php

    <?php

    namespace MyVendor\Bundle\MyBundle\Repository;
    use Doctrine\ODM\PHPCR\DocumentRepository;
    use Symfony\Cmf\Component\Routing\RouteRepositoryInterface;
    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Route as SymfonyRoute;

    class RouteRepository extends DocumentRepository implements RouteRepositoryInterface

    {
        public function findManyByUrl($url)
        {
            $myDocument = $this->findOneBy(array(
                'path' => $url,
            ));

            $pattern = $myDocument->getUrl(); // e.g. "/this/is/a/route"

            $route = new SymfonyRoute($pattern);
            $collection = new RouteCollection(array($route));

            return $collection;
        }

        public function getRouteByName($name, $params = array())
        {
            $route = $this->findOneBy(array(
                'name' => $name,
            ));

            return $route;
        }
    }

The route class
---------------

As noted above, the route classes provided by the route repository must  
be instances of `Symfony\Component\Routing\Route`. A good pattern is to
have your ODM/ORM/Other Document extend the Symfony Route class.

.. code-block:: php

    <?php

    namespace MyVendor\Bundle\MyBundle\Document;
    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;
    use DCMS\Bundle\CoreBundle\Validation\Constraints as RoutingValidation;
    use Symfony\Component\Routing\Route as SymfonyRoute;

    /**
     * @PHPCR\Document(repositoryClass="MyVendor\Bundle\MyBundle\Repository\RouteRepository")
     */
    class Route extends SymfonyRoute
    {
        // @todo: Fill this out
    }

Replacing the default CMF repository
------------------------------------

The final step is to replace the default CMF routing repository service with 
your own. This is easily accomplished using the application configuration:

.. code-block:: yaml

    # app/config/config.yml
    symfony_cmf_routing_extra:
        dynamic:
            enabled: true
            route_repository_service_id: my_bundle.repository.endpoint
   
Where `my_bundle.repository.endpoint` is the service ID of your repository. 
See `Creating and configuring services in the container <http://symfony.com/doc/current/book/service_container.html#creating-configuring-services-in-the-container/>`_ 
for information on creating custom services.
