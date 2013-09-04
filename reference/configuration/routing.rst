RoutingBundle Configuration
===========================

The RoutingBundle contains the Routing logic of the CMF and can be configured
under the ``cmf_routing`` key in your application configuration. When using
XML, you can use the ``http://cmf.symfony.com/schema/dic/routing`` namespace.

Configuration
-------------

chain
~~~~~

.. _config-chain-routers:

routers_by_id
.............

**prototype**: ``array`` **default**: ``{ router.default: 100 }``

This defines the routers to use in the chain. By default, only the Symfony2
router is used. The key is the name of the service and the value is the
priority. The chain is sorted from the highest to the lowest priority.

To add the ``DynamicRouter``, use the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_routing:
            chain:
                routers_by_id:
                    cmf_routing.dynamic_router: 200
                    router.default: 100

    .. code-block:: xml

        <config xmlns="http://cmf.symfony.com/schema/dic/routing">
            <chain>
                <router-by-id id="cmf_routing.dynamic_router">200</router-by-id>
                <router-by-id id="router.default">100</router-by-id>
            </chain>
        </config>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'chain' => array(
                'routers_by_id' => array(
                    'cmf_routing.dynamic_router' => 200,
                    'router.default' => 100,
                ),
            ),
        ));

.. tip::

    You can also add routers to the chain using the ``cmf_routing.router`` tag
    on a service, learn more in ":ref:`routing-chain-router-tag`".

replace_symfony_router
......................

**type**: ``Boolean`` **default**: ``true``

If this option is set to ``false``, the default Symfony2 router will *not* be
overriden by the ``ChainRouter``. By default, the ``ChainRouter`` will
override the default Symfony2 router, but it will pass all requests to the
default router, because :ref:`no other routers were set <config-chain-routers>`.

.. _reference-routing-config-dynamic:

dynamic
~~~~~~~

enabled
.......

**type**: ``Boolean`` **default**: ``false``

Use this setting to activate the ``DynamicRouter``. This will create a
``cmf_routing.dynamic_router`` service which you can add to the chain.

generic_controller
..................

**type**: ``string`` **default**: ``null``

The controller to use when no controllers are configured for the matching
``Route``. The value is the name of a controller using either the
``AcmeDemoBundle::Page::index`` or ``acme_demo.controller.page:indexAction``
notation.

controllers_by_type
...................

**prototype**: ``array``

If the matching route has a ``type`` value in its defaults and no explicit
controller is set, the route is handled by the controller configured for that
type:

.. configuration-block::

    .. code-block:: yaml

        cmf_routing:
            dynamic:
                controllers_by_type:
                    editable: acme_main.controller:editableAction 

    .. code-block:: xml

        <config xmlns="http://cmf.symfony.com/schema/dic/routing">
            <dynamic>
                <controller-by-type type="editable">acme_main.controller:editableAction</controller-by-type>
            </dynamic>
        </config>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'controllers_by_type' => array(
                    'editable' => 'acme_main.controller:editableAction',
                ),
            ),
        ));

controllers_by_class
....................

**prototype**: ``array``

The controller to use when the matching route returns an object for
``getRouteContent()``. This object is checked for being ``instanceof`` the
class names in this map. Instanceof is used instead of direct comparison to
work with proxy classes and other extending classes.

.. configuration-block::

    .. code-block:: yaml

        cmf_routing:
            dynamic:
                controllers_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: cmf_content.controller:indexAction

    .. code-block:: xml

        <config xmlns="http://cmf.symfony.com/schema/dic/routing">
            <dynamic>
                <controller-by-class
                    class="Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent">
                    cmf_content.controller:indexAction
                </controller-by-class>
            </dynamic>
        </config>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'controllers_by_class' => array(
                    'Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent' => 'cmf_content.controller:indexAction',
                ),
            ),
        ));

template_by_class
.................

**prototype**: ``array``

The template to use when the object returning from ``getRouteContent()`` is
``instanceof`` the class names in this map. If matched, the template will be
set as ``_template`` in the defaults and the generic controller is used as
controller.

.. configuration-block::

    .. code-block:: yaml

        cmf_routing:
            dynamic:
                templates_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: CmfContentBundle:StaticContent:index.html.twig

    .. code-block:: xml

        <config xmlns="http://cmf.symfony.com/schema/dic/routing">
            <dynamic>
                <template-by-class
                    class="Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent">
                    CmfContentBundle:StaticContent:index.html.twig
                </template-by-class>
            </dynamic>
        </config>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'templates_by_class' => array(
                    'Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent' => 'CmfContentBundle:StaticContent:index.html.twig',
                ),
            ),
        ));

persistence
...........

phpcr
"""""

enabled
*******

**type**: ``boolean`` **default**: ``false``

If ``true``, PHPCR is enabled in the service container.

manager_name
************

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.enabled``.

route_basepath
**************

**type**: ``string`` **default**: ``/cms/routes``

The basepath for routes in the PHPCR tree.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to
``%cmf_core.persistence.phpcr.basepath%/routes``.

content_basepath
****************

**type**: ``content_basepath`` **default**: ``/cms/content``

The basepath for content objects in the PHPCR tree.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to
``%cmf_core.persistence.phpcr.basepath%/content``.

use_sonata_admin
****************

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the admin classes for the routing are activated on the sonata
admin panel. If set to ``auto``, the admin services are activated only if the
SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to the value
of ``cmf_core.persistence.phpcr.use_sonata_admin``.

orm
"""

enabled
*******

**type**: ``boolean`` **default**: ``false``

If ``true``, PHPCR is included in the service container.

manager_name
************

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use.

uri_filter_regexp
~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``""``

Sets a pattern to which the Route must match before getting the routes from a
database. This can improve the performance a lot when using many routes.

route_provider_service_id
~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string``

When no persistence layer is enabled, a route provider service *must* be
provided in order to get the routes. This is done by using the
``route_provider_service_id`` setting.

route_filters_by_id
~~~~~~~~~~~~~~~~~~~

**prototype**: ``array``

This configures the :ref:`route filters <components-routing-filters>` to use
for filtering the route collection. The key is the id of the service and the
value is the priority. The filters are sorted from the highest to the lowest
priority.

.. configuration-block::

    .. code-block:: yaml

        cmf_routing:
            dynamic:
                route_filters_by_id:
                    acme_main.routing.foo_filter: 100

    .. code-block:: xml
    
        <config xmlns="http://cmf.symfony.com/schema/dic/routing">
            <dynamic>
                <route-filter-by-id id="acme_main.routing.foo_filter">100</route-filter-by-id>
            </dynamic>
        </config>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'route_filters_by_id' => array(
                    'acme_main.routing.foo_filter' => 100,
                ),
            ),
        ));

content_repository_service_id
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``scalar`` **default**: ``null``

To use a content repository when creating URIs, this option can be set to the
content repository service.

.. note::
    
    If PHPCR is enabled, it'll automatically use the phpcr content repository.
    This can be overriden by this option. ORM doesn't have a content
    repository at the moment.

locales
~~~~~~~

**type**: ``array`` **default**: 

To enable multilanguage, set the valid locales in this option.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to the value
of ``cmf_core.locales``.
