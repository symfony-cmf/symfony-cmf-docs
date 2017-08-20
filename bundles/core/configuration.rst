Configuration Reference
=======================

The CoreBundle can be configured under the ``cmf_core`` key in your application
configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/core`` namespace.

Configuration
-------------

Some configuration settings set on the CoreBundle are forwarded as default
configuration to all CMF bundles that are installed in your application. This
is explicitly listed below for each configuration option that is forwarded.

.. _config-core-persistence:

``persistence``
~~~~~~~~~~~~~~~

``phpcr``
.........

This enables the persistence driver for the PHP content repository. The default
configuration is the following:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_core:
            persistence:
                phpcr:
                    enabled:              false
                    basepath:             /cms
                    manager_registry:     doctrine_phpcr
                    manager_name:         ~
                    translation_strategy: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr
                        enabled="false"
                        basepath="/cms"
                        manager-registery="doctrine_phpcr"
                        manager-name="null"
                        translation-strategy="null"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', [
            'persistence' => [
                'phpcr' => [
                    'enabled'              => false,
                    'basepath'             => '/cms/simple',
                    'manager_registry'     => 'doctrine_phpcr',
                    'manager_name'         => null,
                    'translation_strategy' => null,
                ],
            ],
        ]);

``orm``
.......

This enables the persistence driver for relational databases. The default
configuration is the following:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_core:
            persistence:
                orm:
                    enabled:          false
                    manager_name:     ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr
                        enabled="false"
                        manager-name="null"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', [
            'persistence' => [
                'phpcr' => [
                    'enabled'          => false,
                    'manager_name'     => null,
                ],
            ],
        ]);

``enabled``
"""""""""""

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

This setting is propagated as default value to all installed CMF bundles that support
this setting:

* :doc:`BlockBundle <../block/introduction>`
* :doc:`ContentBundle <../content/introduction>`
* :doc:`CreateBundle <../create/introduction>`
* :doc:`MediaBundle <../media/introduction>`
* :doc:`MenuBundle <../menu/introduction>`
* :doc:`RoutingBundle <../routing/introduction>`
* :doc:`SearchBundle <../search/introduction>`
* :doc:`SimpleCmsBundle <../simple_cms/introduction>`

``basepath``
""""""""""""

**type**: ``string`` **default**: ``/cms/``

The basepath for CMS documents in the PHPCR tree.

This setting is propagated as default value to all installed CMF bundles that support
this setting:

* :doc:`BlockBundle <../block/introduction>`
* :doc:`ContentBundle <../content/introduction>`
* :doc:`MediaBundle <../media/introduction>`
* :doc:`MenuBundle <../menu/introduction>`
* :doc:`RoutingBundle <../routing/introduction>`
* :doc:`SearchBundle <../search/introduction>`
* :doc:`SeoBundle <../seo/introduction>`
* :doc:`SimpleCmsBundle <../simple_cms/introduction>`

``manager_registry``
""""""""""""""""""""

**type**: ``string`` **default**: ``doctrine_phpcr``

The doctrine registry from which to get the document manager. This setting
only needs to be changed when configuring multiple manager registries.

This setting is propagated as default value to all installed CMF bundles that support
this setting:

* :doc:`SearchBundle <../search/introduction>`
* :doc:`SimpleCmsBundle <../simple_cms/introduction>`

``manager_name``
""""""""""""""""

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use. ``null`` tells the manager registry to
retrieve the default manager.

This setting is propagated as default value to all installed CMF bundles that support
this setting:

* :doc:`BlockBundle <../block/introduction>`
* :doc:`MediaBundle <../media/introduction>`
* :doc:`MenuBundle <../menu/introduction>`
* :doc:`RoutingBundle <../routing/introduction>`
* :doc:`SearchBundle <../search/introduction>`
* :doc:`SimpleCmsBundle <../simple_cms/introduction>`

``translation_strategy``
""""""""""""""""""""""""

**type**: ``string`` **default**: ``null``

This setting can be used to :ref:`force a specific translation strategy <bundles-core-multilang-global_translation_strategy>`
for all documents.

.. _config-core-multilang:

``multilang``
~~~~~~~~~~~~~

This configures the locales to use in multiple languages mode.

If the ``multilang`` option is *not* defined at all, the CoreBundle registers a
listener for Doctrine PHPCR-ODM that modifies PHPCR-ODM metadata to remove the
translatable attribute from all fields.

If multi-language is enabled, the locales will be configured as default on all
installed CMF bundles that use this configuration:

* :doc:`RoutingBundle <../routing/introduction>`
* :doc:`SimpleCmsBundle <../simple_cms/introduction>`

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_core:
            multilang:
                locales: [en, fr]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <multilang>
                    <locale>en</locale>
                    <locale>fr</locale>
                </multilang>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', [
            'multilang' => [
                'locales' => [
                    'en',
                    'fr',
                ],
            ],
        ]);

``locales``
...........

**type**: ``array`` **default**: ``null``

List of the languages that can be used with the storage.

``publish_workflow``
~~~~~~~~~~~~~~~~~~~~

This configures whether the publish workflow should be enabled, which service to use
and what role may view content not yet published. The request listener ensures
only published routes and content can be accessed.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_core:
            publish_workflow:
                enabled:                 true
                checker_service:         cmf_core.publish_workflow.checker.default
                view_non_published_role: ROLE_CAN_VIEW_NON_PUBLISHED
                request_listener:        true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <publish-workflow
                    enabled="true"
                    checker-service="cmf_core.publish_workflow.checker.default"
                    view-non-published-role="ROLE_CAN_VIEW_NON_PUBLISHED"
                    request-listener="true"
                />
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', [
            'publish_workflow' => [
                'enabled'                 => true,
                'checker_service'         => 'cmf_core.publish_workflow.checker.default',
                'view_non_published_role' => 'ROLE_CAN_VIEW_NON_PUBLISHED',
                'request_listener'        => true,
            ],
        ]);
