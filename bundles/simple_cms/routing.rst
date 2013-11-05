.. index::
    single: Routing; SimpleCmsBundle

Routing in SimpleCmsBundle
--------------------------

SimpleCms provides its own dynamic router service (``cmf_simple_cms.dynamic_router``).
The router loads pages from the basepath provided by the ``cmf_simple_cms.persistence.phpcr.basepath`` 
configuration setting and is an instance of ``Symfony\Cmf\Bundle\RoutingBundle\Routing\DynamicRouter``. 
Just like the normal Dynamic Router, you can register Route Enhancers for the 
SimpleCms router in order to manipulate the parameters of the matching route by 
creating a service that implements ``RouteEnhancerInterface`` and tagging it as 
``dynamic_router_route_enhancer``. For details on Route Enhancers, see the 
documentation for `DynamicRouter`_.

.. _`SimpleCmsBundle`: https://github.com/symfony-cmf/SimpleCmsBundle#readme
.. _`Symfony CMF Standard Edition`: https://github.com/symfony-cmf/symfony-cmf-standard
.. _`CMF website`: https://github.com/symfony-cmf/cmf-website/
