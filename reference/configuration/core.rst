CoreBundle Configuration
========================

The CoreBundle provides infrastructure for other CMF bundles and can be configured
under the ``cmf_core`` key in your application configuration. When using
XML, you can use the ``http://cmf.symfony.com/schema/dic/core`` namespace.

Configuration
-------------

.. _config-core-persistence:

persistence
~~~~~~~~~~~

phpcr
.....

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_core:
            persistence:
                phpcr:
                    enabled:              false
                    basepath:             /cms
                    manager_registry:     doctrine_phpcr
                    manager_name:         ~
                    use_sonata_admin:     auto
                    translation_strategy: ~

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr
                        enabled="false"
                        basepath="/cms"
                        manager-registery="doctrine_phpcr"
                        manager-name="null"
                        use-sonata-admin="auto"
                        translation-strategy="null"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_core', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled'              => false,
                    'basepath'             => '/cms/simple',
                    'manager_registry'     => 'doctrine_phpcr',
                    'manager_name'         => null,
                    'use_sonata_admin'     => 'auto',
                    'translation_strategy' => null,
                ),
            ),
        ));

orm
...

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_core:
            persistence:
                orm:
                    enabled:          false
                    manager_name:     ~
                    use_sonata_admin: auto

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr
                        enabled="false"
                        manager-name="null"
                        use-sonata-admin="auto"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_core', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled'          => false,
                    'manager_name'     => null,
                    'use_sonata_admin' => 'auto',
                ),
            ),
        ));

enabled
"""""""

.. include:: partials/persistence_phpcr_enabled.rst.inc

Enabling this setting will also automatically enable the equivalent setting in the following Bundles:

* :doc:`BlockBundle <../../bundles/block/introduction>`
* :doc:`ContentBundle <../../bundles/content/introduction>`
* :doc:`CreateBundle <../../bundles/create/introduction>`
* :doc:`MediaBundle <../../bundles/media/introduction>`
* :doc:`MenuBundle <../..//bundles/menu/index>`
* :doc:`RoutingBundle <../../bundles/routing/introduction>`
* :doc:`SearchBundle <../../bundles/search/introduction>`
* :doc:`SimpleCmsBundle <../../bundles/simple_cms/introduction>`
* :doc:`TreeBrowserCmsBundle <../../bundles/tree_browser/introduction>`

basepath
""""""""

**type**: ``string`` **default**: ``/cms/``

The basepath for CMS documents in the PHPCR tree.

Enabling this setting will also automatically enable the equivalent settings in the following Bundles:

* :doc:`BlockBundle <../../bundles/block/introduction>`
* :doc:`ContentBundle <../../bundles/content/introduction>`
* :doc:`MediaBundle <../../bundles/media/introduction>`
* :doc:`MenuBundle <../..//bundles/menu/index>`
* :doc:`RoutingBundle <../../bundles/routing/introduction>`
* :doc:`SearchBundle <../../bundles/search/introduction>`
* :doc:`SimpleCmsBundle <../../bundles/simple_cms/introduction>`

manager_registry
""""""""""""""""

**type**: ``string`` **default**: ``doctrine_phpcr``

Enabling this setting will also automatically enable the equivalent settings in the following Bundles:

* :doc:`SearchBundle <../../bundles/search/introduction>`
* :doc:`SimpleCmsBundle <../../bundles/simple_cms/introduction>`

manager_name
""""""""""""

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use. ``null`` tells the manager registry to
retrieve the default manager.<persistence>

Enabling this setting will also automatically enable the equivalent setting in the following Bundles:

* :doc:`BlockBundle <../../bundles/block/introduction>`
* :doc:`MediaBundle <../../bundles/media/introduction>`
* :doc:`MenuBundle <../..//bundles/menu/index>`
* :doc:`RoutingBundle <../../bundles/routing/introduction>`
* :doc:`SearchBundle <../../bundles/search/introduction>`
* :doc:`SimpleCmsBundle <../../bundles/simple_cms/introduction>`

use_sonata_admin
""""""""""""""""

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the admin classes for SimpleCmsBundle pages are activated. If set
to ``auto``, the admin services are activated only if the
SonataPhpcrAdminBundle is present.

Enabling this setting will also automatically enable the equivalent setting in the following Bundles:

* :doc:`BlockBundle <../../bundles/block/introduction>`
* :doc:`ContentBundle <../../bundles/content/introduction>`
* :doc:`MenuBundle <../..//bundles/menu/index>`
* :doc:`RoutingBundle <../../bundles/routing/introduction>`
* :doc:`SimpleCmsBundle <../../bundles/simple_cms/introduction>`

translation_strategy
""""""""""""""""""""

**type**: ``string`` **default**: ``null``

This setting can be used to force a specific translation strategy for all documents.

.. _config-core-multilang:

multilang
~~~~~~~~~

This configures whether multiple languages mode should be activated.

If the ``multilang`` option is *not* defined at all, the CoreBundle registers a
listener for Doctrine PHPCR-ODM that modifies PHPCR-ODM metadata to remove the
translatable attribute from all fields.

If multilang is enabled, the ``TranslatableExtension`` for
``SonataAdminBundle`` is enabled and the locales will be configured on all CMF
bundles that use this configuration:

* :doc:`RoutingBundle <../../bundles/routing/introduction>`
* :doc:`SimpleCmsBundle <../../bundles/simple_cms/introduction>`

.. configuration-block::

    .. code-block:: yaml

        cmf_core:
            multilang:
                locales: [en, fr]

    .. code-block:: xml

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

        $container->loadFromExtension('cmf_core', array(
            'multilang' => array(
                'locales' => array(
                    'en',
                    'fr',
                ),
            ),
        ));

locales
.......

**type**: ``array`` **default**: ``null``

This define languages that can be used.

publish_workflow
~~~~~~~~~~~~~~~~

This configures whether the publish workflow should be enabled, which service to use
and what role may view content not yet published. The request listener ensures
only published routes and content can be accessed.

.. configuration-block::

    .. code-block:: yaml

        cmf_core:
            publish_workflow:
                enabled:                 true
                checker_service:         cmf_core.publish_workflow.checker.default
                view_non_published_role: ROLE_CAN_VIEW_NON_PUBLISHED
                request_listener:        true

    .. code-block:: xml

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

        $container->loadFromExtension('cmf_core', array(
            'publish_workflow' => array(
                'enabled'                 => true,
                'checker_service'         => 'cmf_core.publish_workflow.checker.default',
                'view_non_published_role' => 'ROLE_CAN_VIEW_NON_PUBLISHED',
                'request_listener'        => true,
            ),
        ));

Sonata Admin
------------

This section configures the Sonata Admin Extensions, see:

* :ref:`Publish Workflow Admin Extensions <bundle-core-workflow-admin-extensions>`;
* :ref:`Translatable Admin Extension <bundle-core-translatable-admin-extension>`.
* :ref:`Child Admin Extension <bundle-core-child-admin-extension>`.

.. configuration-block::

    .. code-block:: yaml

        cmf_core:
            sonata_admin:
                extensions:
                    publishable:
                        form_group: form.group_publish_workflow
                    publish_time:
                        form_group: form.group_general
                    translatable:
                        form_group: form.group_general

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <sonata-admin>
                    <extension>
                        <publishable form-group="form.group_publish_workflow" />
                        <publish-time form-group="form.group_general" />
                        <translatable form-group="form.group_general" />
                    </extension>
                </sonata-admin>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_core', array(
            'sonata_admin' => array(
                'extensions' => array(
                    'publishable' => array(
                        'form_group' => 'form.group_publish_workflow',
                    ),
                    'publish_time' => array(
                        'form_group' => 'form.group_general',
                    ),
                    'translatable' => array(
                        'form_group' => 'form.group_general',
                    ),
                ),
            ),
        ));

form_group
~~~~~~~~~~

**type**: ``string`` **default**: as in above example.

Defines which form group the fields from this extension will appear in within
the Sonata Admin edit interface.
