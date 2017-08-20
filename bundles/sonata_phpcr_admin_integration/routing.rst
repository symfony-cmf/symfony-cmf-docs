RoutingBundle
=============

Both content routes and redirect routes can be administrated. When creating
content routes, a tree of content documents is shown. The administrator can
select a content document to bind to the route. This tree is configured with
the ``content_basepath`` setting.

The root path to add routes defaults to the first entry in ``route_basepaths``
configuration of the RoutingBundle. You can overwrite this with the
``basepath`` option if you need a different base path.

Routes can also be handled in the content admin page. The admin integration
provides an admin extension that allows adding routes to any route aware
objects. Another extension provides a frontend link on all admin pages that
objects that refer to routes.

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                routing:
                    enabled: false
                    basepath: ~
                    content_basepath: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/sonata-phpcr-admin-integration">
                <bundles>
                    <routing
                        enabled="false"
                        basepath="null"
                        content-basepath="null"
                    />
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'routing' => [
                    'enabled' => false,
                    'basepath' => null,
                    'content_basepath' => null,
                ],
            ],
        ];

.. include:: ../_partials/sonata_admin_enabled.rst.inc

The routing admins are ``cmf_sonata_phpcr_admin_integration.routing.route_admin``
and ``cmf_sonata_phpcr_admin_integration.routing.redirect_route_admin``.

``basepath``
************

**type**: ``string`` **default**: first value of ``cmf_routing.dynamic.persistence.phpcr.route_basepaths``

The path at which to create routes with Sonata admin. There can be additional
route basepaths for the routing, but you will need to set up separate admin
services to edit those.

``content_basepath``
********************

**type**: ``string`` **default**: ``/ or cmf_content.persistence.phpcr.content_basepath``

The basepath for content objects in the PHPCR tree. This information is used
to offer the correct subtree to select the target content document for a route.

If the :doc:`ContentBundle <../content/introduction>` is registered, this will
default to ``cmf_content.persistence.phpcr.content_basepath``. Otherwise, it
defaults to ``/`` to show the whole tree.

RouteReferrersInterface Admin Extension
---------------------------------------

This bundle provides an extension to edit referring routes for content that
implements the ``RouteReferrersInterface``.

You can define the form group and tab to be used by the field this extension
adds:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                routing:
                    extensions:
                        referrers:
                            form_group: form.group_routing
                            form_tab: form.tab_routing

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="2.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsd="http://www.w3.org/2001/XMLSchema-instance"
            xsd:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://cmf.symfony.com/schema/dic/sonata_admin_integration http://cmf.symfony.com/schema/dic/sonata_admin_integration/sonata_admin_integration.xsd"
        >

            <config xmlns="http://cmf.symfony.com/schema/dic/sonata_admin_integration">
                <bundles>
                    <routing>
                        <extensions>
                            <referrers
                                form-group="form.group_routing"
                                form-tab="form.tab_routing"
                             />
                        </extensions>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'routing' => [
                    'extensions' => [
                        'referrers' => [
                            'form_group' => 'form.group_routing',
                            'form_tag' => 'form.tab_routing',
                        ],
                    ],
                ],
            ],
        ]);

``form_group``
**************

**type**: ``string`` **default**: ``form.group_routing``

.. include:: ../_partials/sonata_admin_form_group.rst.inc

``form_tab``
************

**type**: ``string`` **default**: ``form.tab_routing``

.. include:: ../_partials/sonata_admin_form_tab.rst.inc

Using the extension
~~~~~~~~~~~~~~~~~~~

To activate the extension in your admin classes, define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_sonata_phpcr_admin_integration.routing.extension.route_referrers:
                    implements:
                        - Symfony\Cmf\Component\Routing\RouteReferrersInterface

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <extension id="cmf_sonata_phpcr_admin_integration.routing.extension.route_referrers">
                    <implements>Symfony\Cmf\Component\Routing\RouteReferrersInterface</implements>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Cmf\Bundle\Routing\RedirectRouteInterface;

        $container->loadFromExtension('sonata_admin', [
            'extensions' => [
                'cmf_sonata_phpcr_admin_integration.routing.extension.route_referrers' => [
                    'implements' => [
                        RouteReferrersInterface::class,
                    ],
                ],
            ],
        ]);

See the `Sonata Admin extension documentation`_ for more information.

FrontendLink Admin Extension
----------------------------

This bundle provides an extension to show a button in Sonata Admin, which links
to the frontend representation of a document. Documents which implement the
``RouteReferrersReadInterface`` and Routes themselves are supported.

To activate the extension in your admin classes, define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_sonata_phpcr_admin_integration.routing.extension.frontend_link:
                    implements:
                        - Symfony\Cmf\Component\Routing\RouteReferrersReadInterface
                    extends:
                        - Symfony\Component\Routing\Route

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <config xmlns="http://sonata-project.org/schema/dic/admin">
            <!-- ... -->
            <extension id="cmf_sonata_phpcr_admin_integration.routing.extension.frontend_link">
                <implements>Symfony\Cmf\Component\Routing\RouteReferrersReadInterface</implements>
                <extends>Symfony\Component\Routing\Route</extends>
            </extension>
        </config>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Cmf\Component\Routing\RouteReferrersReadInterface;
        use Symfony\Component\Routing\Route;

        $container->loadFromExtension('sonata_admin', [
            'extensions' => [
                'cmf_sonata_phpcr_admin_integration.routing.extension.frontend_link' => [
                    'implements' => [
                        RouteReferrersReadInterface::class,
                    ],
                    'extends' => [
                        Route::class,
                    ],
                ],
            ],
        ]);

See the `Sonata Admin extension documentation`_ for more information.

Styling the Frontend Link
*************************

The frontend link button can be customized using the
``.sonata-admin-frontend-link`` selector. For instance, to show a link icon and
make the label bold, use the following CSS:

.. code-block:: css

    .sonata-admin-menu-item a.sonata-admin-frontend-link {
        font-weight: bold;
    }

    .sonata-admin-menu-item a.sonata-admin-frontend-link:before {
        font-family: FontAwesome;
        content: "\f08e";
    }

.. _`Sonata Admin extension documentation`: https://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
