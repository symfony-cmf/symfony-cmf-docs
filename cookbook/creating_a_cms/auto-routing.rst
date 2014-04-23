Routing and Automatic Routing
-----------------------------

The routes (URLs) to your content will be automatically created and updated
using the RoutingAutoBundle. This bundle uses a configuration language to
specify automatic creation of routes, which can be a bit hard to grasp the
first time you see it.

For a full explanation refer to the
:doc:`../../bundles/routing_auto/index`.

In summary, you will configure the auto routing system to create a new auto
routing document in the routing tree for every post or content created. The
new route will be linked back to the target content:

.. image:: ../../_images/cookbook/basic-cms-objects.png

The paths above represent the path in the PHPCR-ODM document tree. In the next
section you will define ``/cms/routes`` as the base path for routes, and subsequently
the contents will be available at the following URLs:

* **Home**: ``http://localhost:8000/page/home``
* **About**: ``http://localhost:8000/page/about``
* etc.

Installation
~~~~~~~~~~~~

Ensure that you have the following package installed:

.. code-block:: javascript

    {
        ...
        require: {
            ...
            "symfony-cmf/routing-auto-bundle": "1.0.*@alpha"
        },
        ...
    }

.. note::

    You are installing the bleeding edge version of the routing-auto bundle.

Enable the routing bundles to your kernel::

    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Cmf\Bundle\RoutingBundle\CmfRoutingBundle(),
                new Symfony\Cmf\Bundle\RoutingAutoBundle\CmfRoutingAutoBundle(),
            );

            // ...
        }
    }

.. note:: 

    The `symfony-cmf/routing-bundle` package is installed automatically as
    `symfony-cmf/routing-auto-bundle` depends on it.

Enable the Dynamic Router
~~~~~~~~~~~~~~~~~~~~~~~~~

The RoutingAutoBundle uses the CMF `RoutingBundle`_ which enables routes to
be provided from a database (in addition to being provided from
the routing configuration files as in core Symfony 2).

Add the following to your application configuration:

.. configuration-block::

    .. code-block:: yaml

        # /app/config/config.yml
        cmf_routing:
            chain:
                routers_by_id:
                    cmf_routing.dynamic_router: 20
                    router.default: 100
            dynamic:
                enabled: true
                persistence:
                    phpcr:
                        route_basepath: /cms/routes

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <chain>
                    <router-by-id id="cmf_routing.dynamic_router">20</router-by-id>
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
            'dynamic' => array(
                'persistence' => array(
                    'phpcr' => array(
                        'enabled' => true,
                        'route_basepath' => '/cms/routes',
                    ),
                ),
            ),
        ));

This will:

#. Cause the default Symfony router to be replaced by the chain router. The
   chain router enables you to have multiple routers in your application. You
   add the dynamic router (which can retrieve routes from the database) and
   the default Symfony router (which retrieves routes from configuration
   files). The number indicates the order of precedence - the router with the
   lowest number will be called first;
#. Configure the **dynamic** router which you have added to the router chain.
   You specify that it should use the PHPCR backend and that the *root* route
   can be found at ``/cms/routes``.

Auto Routing Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~

Create the following file in your applications configuration directory:

.. code-block:: yaml

    # app/config/routing_auto.yml
    cmf_routing_auto:
        mappings:
            Acme\BasicCmsBundle\Document\Page:
                content_path:
                    pages:
                        provider: [specified, { path: /cms/routes/page }]
                        exists_action: use
                        not_exists_action: create
                content_name:
                    provider: [content_method, { method: getTitle }]
                    exists_action: auto_increment
                    not_exists_action: create

            Acme\BasicCmsBundle\Document\Post:
                content_path:
                    blog_path:
                        provider: [specified, { path: /cms/routes/post }]
                        exists_action: use
                        not_exists_action: create
                    date:
                        provider: [content_datetime, { method: getDate}]
                        exists_action: use
                        not_exists_action: create
                content_name:
                    provider: [content_method, { method: getTitle }]
                    exists_action: auto_increment
                    not_exists_action: create

This will configure the routing auto system to automatically create and update
route documents for both the ``Page`` and ``Post`` documents. 

In summary:

* The ``content_path`` key represents the parent path of the content, e.g.
  ``/if/this/is/a/path`` then the ``content_path``
  represents ``/if/this/is/a``;
* Each element under ``content_path`` represents a section of the URL;
* The first element ``blog_path`` uses a *provider* which *specifies* a
  path. If that path exists then it will do nothing;
* The second element uses the ``content_datetime`` provider, which will
  use a ``DateTime`` object returned from the specified method on the
  content object (the ``Post``) and create a path from it, e.g.
  ``2013/10/13``;
* The ``content_name`` key represents the last part of the path, e.g. ``path``
  from ``/if/this/is/a/path``.

Now you will need to include this configuration:

.. configuration-block::
    
    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: routing_auto.yml }

    .. code-block:: xml

        <!-- src/Acme/BasicCmsBUndle/Resources/config/config.yml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container 
            xmlns="http://symfony.com/schema/dic/services" 
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
            xsi:schemaLocation="http://symfony.com/schema/dic/services 
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <import resource="routing_auto.yml"/>
        </container>
    
    .. code-block:: php

        // src/Acme/BasicCmsBundle/Resources/config/config.php

        // ...
        $this->import('routing_auto.yml');

and reload the fixtures:

.. code-block:: bash

    $ php app/console doctrine:phpcr:fixtures:load

Have a look at what you have:

.. code-block:: bash

    $ php app/console doctrine:phpcr:node:dump
    ROOT:
      cms:
        pages:
          Home:
        routes:
          page:
            home:
          post:
            2013:
              10:
                12:
                  my-first-post:
                  my-second-post:
                  my-third-post:
                  my-forth-post:
        posts:
          My First Post:
          My Second Post:
          My Third Post:
          My Forth Post:

The routes have been automatically created!

.. _`routingautobundle documentation`: http://symfony.com/doc/current/cmf/bundles/routing_auto.html
.. _`SonataDoctrinePhpcrAdminBundle`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle
.. _`routingbundle`: http://symfony.com/doc/master/cmf/bundles/routing/index.html
