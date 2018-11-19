ContentBundle
=============

    This integration becomes available once the :doc:`CmfContentBundle
    <../content/introduction>` is installed.

Content bundle support consists in an admin interface for the ``StaticContent``
document. Enable this admin using:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                content: ~

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
                    <content/>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'content' => true,
            ],
        ]);

WYSIWYG Editing with CKEditor
-----------------------------

The content admin comes with an optional integration for the CK WYSIWYG
editor. To use CK instead of plain text, install the IvoryCKEditorBundle_:

    .. code-block:: bash

        $ composer require egeloen/ckeditor-bundle

When using the editor, you need to specify the name of the editor configuration
to use (see the IvoryCKEditorBundle_ documentation for more information on this
configuration):


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                content:
                    ivory_ckeditor:
                        enabled: true
                        config_name: cmf_sonata_phpcr_admin_integration

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
                    <content>
                        <ivory-ckeditor enabled="true" config-name="cmf_sontata_phpcr_admin_integration"/>
                    </content>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'content' => [
                    'ivory_ckeditor' => [
                        'enabled' => true,
                        'config_name' => 'cmf_sonata_phpcr_admin_integration',
                    ],
                ],
            ],
        ]);

.. _IvoryCKEditorBundle: https://github.com/egeloen/IvoryCKEditorBundle
