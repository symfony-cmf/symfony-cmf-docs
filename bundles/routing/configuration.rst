Configuration Reference
=======================

The RoutingBundle can be configured under the ``cmf_routing`` key in your
application configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/routing`` namespace.

Configuration
-------------

``chain``
~~~~~~~~~

.. _reference-config-routing-chain_routers:

``routers_by_id``
.................

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
                    router.default:             100

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <chain>
                    <router-by-id id="cmf_routing.dynamic_router">200</router-by-id>
                    <router-by-id id="router.default">100</router-by-id>
                </chain>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'chain' => array(
                'routers_by_id' => array(
                    'cmf_routing.dynamic_router' => 200,
                    'router.default'             => 100,
                ),
            ),
        ));

.. tip::

    You can also add routers to the chain using the ``cmf_routing.router`` tag
    on a service, learn more in ":ref:`routing-chain-router-tag`".

``replace_symfony_router``
..........................

**type**: ``Boolean`` **default**: ``true``

If this option is set to ``false``, the default Symfony2 router will *not* be
overridden by the ``ChainRouter``. By default, the ``ChainRouter`` will
override the default Symfony2 router, but it will pass all requests to the
default router, because :ref:`no other routers were set <reference-config-routing-chain_routers>`.


.. configuration-block::

    .. code-block:: yaml

        cmf_routing:
            chain:
                replace_symfony_router: true

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <cmf-routing>
                    <chain replace-symfony-router="true"/>
                </cmf-routing>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'chain' => array(
                'replace_symfony_router' => true,
            ),
        ));


.. _reference-config-routing-dynamic:

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

The controller to use when the route enhancers only determined the template but
no explicit controller. The value is the name of a controller using either the
``AcmeDemoBundle::Page::index`` or ``acme_demo.controller.page:indexAction``
notation.

If the :doc:`CoreBundle <../core/introduction>` and
:doc:`ContentBundle <../content/introduction>` are registered, this
defaults to ``cmf_content.controller:indexAction``.

``default_controller``
......................

**type**: ``string`` **default**: value of ``generic_controller``

The default controller to use if none of the enhancers found a controller. The
value is the name of a controller using either the
``AcmeDemoBundle::Page::index`` or ``acme_demo.controller.page:indexAction``
notation.

``controllers_by_type``
.......................

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


        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <controller-by-type type="editable">acme_main.controller:editableAction</controller-by-type>
                </dynamic>
            </config>

        </container>

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

The controller to use when the matching route implements
``RouteObjectInterface`` and returns an object for ``getRouteContent()``.
This object is checked for being ``instanceof`` the class names in this map.
``instanceof`` is used instead of direct comparison to work with proxy classes and
other extending classes. The order in which the classes are specified, matters.
The first match is taken.

If matched, the controller will be set as ``_controller``, making Symfony2
choose this controller to handle the request.

.. configuration-block::

    .. code-block:: yaml

        cmf_routing:
            dynamic:
                controllers_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: cmf_content.controller:indexAction

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <controller-by-class
                        class="Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent">
                        cmf_content.controller:indexAction
                    </controller-by-class>
                </dynamic>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'controllers_by_class' => array(
                    'Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent' => 'cmf_content.controller:indexAction',
                ),
            ),
        ));

.. _reference-config-routing-template_by_class:

``template_by_class``
.....................

**prototype**: ``array``

The template to use when the route implements ``RouteObjectInterface`` and
returns an object for ``getRouteContent()``. This object is checked for being
``instanceof`` the class names in this map. ``instanceof`` is used instead of
direct comparison to work with proxy classes and other extending classes. The
order in which the classes are specified, matters. The first match is taken.

If matched, the template will be set as ``_template`` in the defaults and
unless another mapping specifies a controller, the ``generic_controller``
setting is set as controller.

.. configuration-block::

    .. code-block:: yaml

        cmf_routing:
            dynamic:
                templates_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: CmfContentBundle:StaticContent:index.html.twig

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <template-by-class
                        class="Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent">
                        CmfContentBundle:StaticContent:index.html.twig
                    </template-by-class>
                </dynamic>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'templates_by_class' => array(
                    'Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent' => 'CmfContentBundle:StaticContent:index.html.twig',
                ),
            ),
        ));

``route_collection_limit``
..........................

**type**: ``scalar``, **default**: ``0``

If this value is set to a number bigger than 0, the routes from the database
are returned in the ``getRouteCollection``. The limit serves to prevent huge
route lists if you have a large database. Setting the limit to ``false``
disables the limit entirely.

``persistence``
...............

.. _reference-configuration-routing-persistence-phpcr:

``phpcr``
"""""""""

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                persistence:
                    phpcr:
                        enabled:      false
                        manager_name: ~
                        route_basepaths:
                            - /cms/routes
                            - /cms/simple
                        content_basepath: /cms/content
                        admin_basepath:   /cms/routes
                        use_sonata_admin: auto
                        enable_initializer: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <persistence>
                        <phpcr
                            enabled="false"
                            manager-name="null"
                            content-basepath="/cms/content"
                            admin-basepath="/cms/routes"
                            use-sonata-admin="auto"
                            enable_initializer="true"
                        >
                            <route-basepath>/cms/routes</route-basepath>
                            <route-basepath>/cms/simple</route-basepath>
                        </phpcr>
                    </persistence>
                </dynamic>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'persistence' => array(
                    'phpcr' => array(
                        'enabled'            => false,
                        'manager_name'       => null,
                        'route_basepaths'    => array(
                            '/cms/routes',
                            '/cms/simple',
                        )
                        'content_basepath'   => '/cms/content',
                        'admin_basepath'     => '/cms/routes',
                        'use_sonata_admin'   => 'auto',
                        'enable_initializer' => true,
                    ),
                ),
            ),
        ));

enabled
*******

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

manager_name
************

.. include:: ../_partials/persistence_phpcr_manager_name.rst.inc

``route_basepaths``
*******************

.. versionadded:: 1.3
    The ``route_basepaths`` setting was introduced in version 1.3. Prior to
    1.3, you could only configure one basepath using ``route_basepath``.

**type**: ``string`` | ``array`` **default**: ``/cms/routes``

A set of paths where routes are located in the PHPCR tree.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will
default to ``%cmf_core.persistence.phpcr.basepath%/routes``. If the
:doc:`SimpleCmsBundle <../simple_cms/introduction>` is registered as well,
the SimpleCmsBundle basepath will be added as an additional route basepath.

``content_basepath``
********************

**type**: ``string`` **default**: ``/cms/content``

The basepath for content objects in the PHPCR tree. This information is used
with the sonata admin to offer the correct subtree to select content documents.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to
``%cmf_core.persistence.phpcr.basepath%/content``.

``admin_basepath``
******************

**type**: ``string`` **default**: first value of ``route_basepaths``

The path at which to create routes with Sonata admin. There can be additional
route basepaths, but you will need your own tools to edit those.

``use_sonata_admin``
********************

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the admin classes for the routing are loaded and available for
sonata. If set to ``auto``, the admin services are activated only if the
SonataPhpcrAdminBundle is present.

.. note::

    To see the route administration on the sonata dashboard, you still need to
    configure it to show the items ``cmf_routing.route_admin`` and
    ``cmf_routing.redirect_route_admin``.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will
default to the value of ``cmf_core.persistence.phpcr.use_sonata_admin``.

``enable_initializer``
**********************

**type**: ``boolean`` **default**: ``true``

.. versionadded:: 1.3
    This configuration option was introduced in RoutingBundle 1.3.

The bundle comes with an initializer that creates the nodes for the ``admin_basepath``
automatically when initializing the repository or loading fixtures. Sometimes this
is not what you want, as the created node is of type 'Generic' and sometimes this
already needs to be a route (for the homepage). Set this to false to disable the
initializer when you create your nodes your self (e.g. using Alice_).

.. caution::

    Initializers are forced to be disabled when Sonata Admin is not enabled.
    In such cases, you might have multiple route basepaths which are created
    by other sources. If the route basepath isn't created by another source,
    you have to configure an :ref:`initializer <phpcr-odm-repository-initializers>`.

``orm``
"""""""

``enabled``
***********

**type**: ``boolean`` **default**: ``false``

If ``true``, the ORM is included in the service container.

``manager_name``
****************

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use.

``uri_filter_regexp``
~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``""``

Sets a pattern to which the Route must match before attempting to get any
routes from a database. This can improve the performance a lot when only a
subsection of your site is using the dynamic router.

``route_provider_service_id``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string``

When none of the persistence layers is enabled, a route provider service *must*
be provided in order to get the routes. This is done by using the
``route_provider_service_id`` setting.

``route_filters_by_id``
~~~~~~~~~~~~~~~~~~~~~~~

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


        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <route-filter-by-id id="acme_main.routing.foo_filter">100</route-filter-by-id>
                </dynamic>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'route_filters_by_id' => array(
                    'acme_main.routing.foo_filter' => 100,
                ),
            ),
        ));

``content_repository_service_id``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``scalar`` **default**: ``null``

To use a content repository when creating URLs, this option can be set to the
content repository service.

.. note::

    If PHPCR is enabled, it'll automatically use the phpcr content repository.
    This can be overridden by this option. ORM doesn't have a content
    repository at the moment.

.. _reference-config-routing-locales:

``url_generator``
~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``cmf_routing.generator``

Service id for the DynamicRouter to generate URLs from Route objects. Overwrite
to a service implementing ``UrlGeneratorInterface`` if you need to customize that
service.

``locales``
~~~~~~~~~~~

**type**: ``array`` **default**: ``array()``

To enable multi-language, set the valid locales in this option.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will
default to the value of ``cmf_core.locales``.

``limit_candidates``
~~~~~~~~~~~~~~~~~~~~

**type**: ``integer`` **default**: ``20``

With this flag you can tune the routing behavior when using the dynamic
pattern part of routes stored in the database. If you do never use the variable
pattern field of the Route model, you can set this to 1 as a small performance
optimization. If you have very complex URLs with patterns, you might need to
increase the limit.

.. caution::

    Setting this to a higher makes your site more vulnerable to load attacks
    when someone visits your site with URLs with lots of slashes in them, since
    every slash will lead to a document being tried to be loaded.

``match_implicit_locale``
~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``boolean`` **default**: ``true``

Whether the route provider should look for routes without the locale.

For example, when the ``locales`` are ``de`` and ``en`` and the request has the
url ``de/my/path``, the route provider will not only look for ``de/my/path``,
``de/my`` and ``de`` but also for ``my/path`` and ``my``. This allows to use a
single route for multiple languages. This is used for example by the
:doc:`SimpleCms <../simple_cms/introduction>`.

If you do not need this, disabling the option will gain some performance.

``auto_locale_pattern``
~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``boolean`` **default**: ``false``

If you enable this option, the LocaleListener will ensure that routes that have
no locale in their static pattern get the ``auto_locale_pattern`` option set.

.. note::

    Enabling this option will prevent you from having any CMF Routes that match
    URL without a locale in it.

This is ignored if there are no ``locales`` configured. It makes no sense to
enable this option when ``match_implicit_locale`` is disabled.

.. _Alice: https://github.com/nelmio/alice
