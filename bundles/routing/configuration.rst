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

This defines the routers to use in the chain. By default, only the Symfony
router is used. The key is the name of the service and the value is the
priority. The chain is sorted from the highest to the lowest priority.

To add the ``DynamicRouter``, use the following configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            chain:
                routers_by_id:
                    cmf_routing.dynamic_router: 200
                    router.default:             100

    .. code-block:: xml

        <!-- app/config/config.xml -->
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

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', [
            'chain' => [
                'routers_by_id' => [
                    'cmf_routing.dynamic_router' => 200,
                    'router.default'             => 100,
                ],
            ],
        ]);

.. tip::

    You can also add routers to the chain using the ``cmf_routing.router`` tag
    on a service, learn more in ":ref:`routing-chain-router-tag`".

``replace_symfony_router``
..........................

**type**: ``Boolean`` **default**: ``true``

If this option is set to ``false``, the default Symfony router will *not* be
overridden by the ``ChainRouter``. By default, the ``ChainRouter`` will
override the default Symfony router, but it will pass all requests to the
default router, because :ref:`no other routers were set <reference-config-routing-chain_routers>`.


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            chain:
                replace_symfony_router: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <cmf-routing>
                    <chain replace-symfony-router="true"/>
                </cmf-routing>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', [
            'chain' => [
                'replace_symfony_router' => true,
            ],
        ]);


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

This configuration specifies the controller that is used when the route
enhancers define a template but no explicit controller. It accepts any valid
Symfony controller reference.

If the :doc:`CoreBundle <../core/introduction>` and
:doc:`ContentBundle <../content/introduction>` are registered, this
defaults to ``cmf_content.controller:indexAction``.

``default_controller``
......................

**type**: ``string`` **default**: value of ``generic_controller``

The default controller to use if none of the enhancers found a controller.
Accepts any valid Symfony controller reference.


``controllers_by_type``
.......................

**prototype**: ``array``

If the matching route has a ``type`` value in its defaults and no explicit
controller is set, the route is handled by the controller configured for that
type:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                controllers_by_type:
                    editable:   AppBundle:Cms:editable

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <controller-by-type type="editable">AppBundle:Cms:editable</controller-by-type>
                </dynamic>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', [
            'dynamic' => [
                'controllers_by_type' => [
                    'editable' => 'AppBundle:Cms:editable',
                ],
            ],
        ]);

controllers_by_class
....................

**prototype**: ``array``

The controller to use when the matching route implements
``RouteObjectInterface`` and returns an object for ``getRouteContent()``.
This object is checked for being ``instanceof`` the class names in this map.
``instanceof`` is used instead of direct comparison to work with proxy classes and
other extending classes. The order in which the classes are specified, matters.
The first match is taken.

If matched, the controller will be set as ``_controller``, making Symfony
choose this controller to handle the request.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                controllers_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: cmf_content.controller:indexAction

    .. code-block:: xml

        <!-- app/config/config.xml -->
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

        // app/config/config.php
        use Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent;

        $container->loadFromExtension('cmf_routing', [
            'dynamic' => [
                'controllers_by_class' => [
                    StaticContent::class => 'cmf_content.controller:indexAction',
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

        # app/config/config.yml
        cmf_routing:
            dynamic:
                templates_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: CmfContentBundle:StaticContent:index.html.twig

    .. code-block:: xml

        <!-- app/config/config.xml -->
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

        // app/config/config.php
        use Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent;

        $container->loadFromExtension('cmf_routing', [
            'dynamic' => [
                'templates_by_class' => [
                    StaticContent::class => 'CmfContentBundle:StaticContent:index.html.twig',
                ),
            ),
        ));

``route_collection_limit``
..........................

**type**: ``scalar``, **default**: ``0``

If this value is set to a number bigger than 0, the ``getRouteCollection``
method returns a collection of routes read from the database. The limit serves
to prevent huge route lists if you have a large database. Setting the limit to
``false`` disables the limit entirely and attempts to return all routes.

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

        # app/config/config.php
        $container->loadFromExtension('cmf_routing', [
            'dynamic' => [
                'persistence' => [
                    'phpcr' => [
                        'enabled'            => false,
                        'manager_name'       => null,
                        'route_basepaths'    => [
                            '/cms/routes',
                            '/cms/simple',
                        ],
                        'enable_initializer' => true,
                    ],
                ],
            ],
        ]);

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
default to ``%cmf_core.persistence.phpcr.basepath%/routes``.

``enable_initializer``
**********************

**type**: ``boolean`` **default**: ``true``

.. versionadded:: 1.3
    This configuration option was introduced in RoutingBundle 1.3.

The bundle comes with an initializer that creates the necessary nodes for all
``route_basepaths`` roots to exist automatically when initializing the
repository or loading fixtures. Sometimes this is not what you want, as the
created node is of type 'Generic' and you might want the document to be a route
(for the homepage). Set this to false to disable the initializer when you
create your nodes yourself in your own :ref:`initializer <phpcr-odm-repository-initializers>`.

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

``route_class``
***************

**type**: ``string`` **default**: ``'Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Orm\Route'``

The route class to use in your application.

.. versionadded:: 2.0
    The ``route_class`` parameter has been added in version 2.0.

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

        # app/config/config.yml
        cmf_routing:
            dynamic:
                route_filters_by_id:
                    app.routing_filter: 100

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <route-filter-by-id id="app.routing_filter">100</route-filter-by-id>
                </dynamic>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', [
            'dynamic' => [
                'route_filters_by_id' => [
                    'app.routing_filter' => 100,
                ],
            ],
        ]);

``content_repository_service_id``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``scalar`` **default**: ``null``

One way for routes to specify content is by specifying the content ID. The
responsible route enhancer asks the content repository specified here for the
content. The repository has to implement ``Symfony\Cmf\Component\Routing\ContentRepositoryInterface``.

.. note::

    If PHPCR is enabled, this setting will default to a generic PHPCR content
    repository that tries to use the content ID as PHPCR path. ORM doesn't have
    a content repository at the moment.

.. _reference-config-routing-locales:

``url_generator``
~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``cmf_routing.generator``

Service id for the DynamicRouter to generate URLs from Route objects. Overwrite
to a service implementing ``UrlGeneratorInterface`` if you need to customize that
service.

``locales``
~~~~~~~~~~~

**type**: ``array`` **default**: ``[]``

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
