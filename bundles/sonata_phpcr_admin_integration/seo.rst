SeoBundle
=========

The SeoBundle has no documents of its own and thus no stand alone admins. For
the ``SeoAwareInterface``, we provide an admin extension that uses the
:doc:`form <../seo/seo_aware>` provided by the SeoBundle. This allows to edit a
``SeoMetadata`` object in admins for documents that implement seo aware.

.. caution::

    The Sonata Admin uses the form type provided by the CmfSeoBundle. That form
    type requires to have the `BurgovKeyValueFormBundle`_ installed.

Configuration
-------------

This section configures the admin extension for SEO aware content.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                seo:
                    enabled: true
                    extensions:
                        metadata:
                            form_group: form.group_seo
                            form_tab: form.tab_seo

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/sonata-phpcr-admin-integration">
                <bundles>
                    <seo enabled="true">
                        <extensions>
                            <metadata
                                form-group="form.group_seo"
                                form-tab="form.tab_seo"
                            />
                        </extensions>
                    </seo>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'seo' => [
                    'enabled' => true,
                    'extensions' => [
                        'metadata' => [
                            'form_group' => 'form.group_seo',
                            'form_tab' => 'form.tab_seo',
                        ],
                    ],
                ],
            ],
        ];

.. include:: ../_partials/sonata_admin_enabled.rst.inc

``form_group``
~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``form.group_seo``

.. include:: ../_partials/sonata_admin_form_group.rst.inc

``form_tab``
~~~~~~~~~~~~

**type**: ``string`` **default**: ``form.tab_seo``

.. include:: ../_partials/sonata_admin_form_tab.rst.inc

SeoAwareInterface Admin Extension
---------------------------------

This extension allows to edit ``SeoMetadata`` in any admin for a document that
implements ``SeoAwareInterface``.

To activate the extension in your admin classes, define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_sonata_phpcr_admin_integration.seo.extension.metadata:
                    implements:
                        - Symfony\Cmf\Bundle\SeoBundle\SeoAwareInterface

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <extension id="cmf_sonata_phpcr_admin_integration.seo.extension.metadata">
                    <implements>Symfony\Cmf\Bundle\SeoBundle\SeoAwareInterface</implements>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Cmf\Bundle\SeoBundle\SeoAwareInterface;

        $container->loadFromExtension('sonata_admin', [
            'extensions' => [
                'cmf_sonata_phpcr_admin_integration.seo.extension.metadata' => [
                    'implements' => [
                        SeoAwareInterface::class,
                    ],
                ],
            ],
        ]);

See the `Sonata Admin extension documentation`_ for more information.

.. _`BurgovKeyValueFormBundle`: https://github.com/Burgov/KeyValueFormBundle
.. _`Sonata Admin extension documentation`: https://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
