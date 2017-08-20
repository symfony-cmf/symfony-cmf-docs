Configuration Reference
=======================

The RoutingAutoBundle can be configured under the ``cmf_routing_auto`` key in your
application configuration.

Configuration
-------------

``adapter``
~~~~~~~~~~~

**type**: ``scalar`` **default**: ``doctrine_phpcr_odm`` if ``persistence`` configuration option is set to ``phpcr``

Alias of the :doc:`adapter <adapter>` used to manage routes.

``auto_mapping``
~~~~~~~~~~~~~~~~

**type**: ``boolean`` **default**: ``true``

Look for the configuration file ``cmf_routing_auto.yml`` in `Resource/config` folder of all
available bundles.

``mapping``
~~~~~~~~~~~

Specify files with auto routing mapping.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing_auto:
            mapping:
                resources:
                    -
                        path: app/Resources/routing_auto.yml
                        type: yaml

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing_auto">
                <mapping>
                    <resource path="app/Resources/routing_auto.yml" type="yaml"/>
                </mapping>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing_auto', [
            'mapping' => [
                'resources' => [
                    [
                        'path' => 'app/Resources/routing_auto.yml',
                        'type' => 'yaml',
                    ],
                ],
            ],
        ]);

``path``
........

**type**: ``scalar`` **required**

Path to the auto route mapping file in the project.

``type``
........

**type**: ``enum`` **values**: ``yaml``|``xml`` **optional**

Type of the configuration file, for the Symfony configuration loader.

``persistence``
~~~~~~~~~~~~~~~

Select persistence mode. Only PHPCR is provided by this bundle.

``phpcr``
~~~~~~~~~

Use this section to configure the PHPCR adapter.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing_auto:
            persistence:
                phpcr:
                    enabled: true
                    route_basepath: /cms/routes

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing_auto">
                <persistence>
                    <phpcr enabled="true" route-basepath="/cms/routes"/>
                </persistence>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing_auto', [
            'persistence' => [
                'phpcr' => [
                    'enabled' => true,
                    'route_basepath' => '/cms/routes',
                ],
            ],
        ]);

``enabled``
...........

**type**: ``Boolean`` **default**: ``false``

``route_basepath``
..................

**type**: ``scalar`` **default**: ``/cms/routes``

Path to the root route to know where to add auto routes.
