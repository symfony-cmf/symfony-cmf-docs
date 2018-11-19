BlockBundle
===========

This bundle provides Admin classes for all block types. Additionally, it
provides an admin extension to generically edit block options.

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                block:
                    enabled: false
                    basepath: ~
                    enable_menu: auto
                    menu_basepath: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/sonata-phpcr-admin-integration">
                <bundles>
                    <block
                        enabled="false"
                        basepath="null"
                        enable-menu="auto"
                        menu-basepath="null"
                    />
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'block' => [
                    'enabled' => false,
                    'basepath' => null,
                    'enable_menu' => 'auto',
                    'menu_basepath' => null,
                ],
            ],
        ];

.. include:: ../_partials/sonata_admin_enabled.rst.inc

Each admin service name follows the naming pattern
``cmf_sonata_phpcr_admin_integration.block.<type>_admin``. For example, the
slideshow admin service is called ``cmf_sonata_phpcr_admin_integration.block.slideshow_admin``.

``basepath``
************

**type**: ``string`` **default**: first value of ``cmf_block.persistence.phpcr.block_basepaths``

The path at which to create blocks with Sonata admin when you don't attach them to documents.

``menu_enabled``
****************

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

Whether to enable the menu block. This requires the CmfMenuBundle to be
available in your application. When left at ``auto``, the menu block is
automatically enabled if the CmfMenuBundle is available.

``menu_basepath``
*****************

**type**: ``string`` **default**: ``cmf_menu.persistence.phpcr.menu_basepath``

The basepath to select the menu subtree for a menu block. Only useful if
``menu_enabled`` is not false.

.. _bundles-block-types-admin_extension:

Sonata Admin Extension for Basic Block Settings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This bundle provides an extension to edit basic settings of block options like
the time to life (TTL, for caching purposes).

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                block:
                    extensions:
                        block_cache:
                            form_group: form.group_metadata
                            form_tab:   form.tab_general

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/sonata-phpcr-admin-integration">
                <bundles>
                    <block>
                        <extensions>
                            <block-cache form-group="form.group_metadata" form-tab="form.tab_general"/>
                        </extensions>
                    </block>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'block' => [
                    'extensions' => [
                        'block_cache' => [
                            'form_group' => 'form.group_metadata',
                            'form_tab' => 'form.tab_general',
                        ],
                    ],
                ],
            ],
        ];


``form_group``
**************

**type**: ``string`` **default**: ``form.group_metadata``

.. include:: ../_partials/sonata_admin_form_group.rst.inc

``form_tab``
************

**type**: ``string`` **default**: ``form.tab_general``

.. include:: ../_partials/sonata_admin_form_tab.rst.inc

Using the extension
~~~~~~~~~~~~~~~~~~~

To activate the extension in your admin classes, define the extension
configuration in the ``sonata_admin`` section of your project configuration.

Assuming your blocks extend the ``BaseBlock`` class (as all blocks provided by
the CMF BlockBundle do), you can add the following lines to your sonata admin
configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            extensions:
                cmf_block.admin_extension.cache:
                    extends:
                        - Symfony\Cmf\Bundle\BlockBundle\Model\AbstractBlock

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <extension id="cmf_block.admin_extension.cache">
                    <extend>Symfony\Cmf\Bundle\BlockBundle\Model\AbstractBlock</extend>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            'extensions' => array(
                'cmf.block.admin_extension.cache' => array(
                    'extends' => array(
                        Symfony\Cmf\Bundle\BlockBundle\Model\AbstractBlock::class,
                    ),
                ),
            ),
        ));

See the `Sonata Admin extensions documentation`_ for more information.

.. _`Sonata Admin extensions documentation`: https://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
