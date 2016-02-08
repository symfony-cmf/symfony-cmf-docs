``ivory_ckeditor``
~~~~~~~~~~~~~~~~~~

.. versionadded:: 1.3
    IvoryCKEditorBundle integration was introduced in version 1.3.

This configures integration of the IvoryCKEditorBundle_, so an admin can edit
content using CKeditor.

.. configuration-block::

    .. code-block:: yaml

        cmf_content:
            ivory_ckeditor:
                enabled:     auto
                config_name: cmf_content

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-Instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
            http://cmf.symfony.com/schema/dic/content http://cmf.symfony.com/schema/dic/content/content-1.0.xsd"
        >

            <config xmlns="http://cmf.symfony.com/schema/dic/content">
                <ivory-ckeditor enabled="auto" config-name="cmf_content" />
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_content', array(
            'ivory_ckeditor' => array(
                'enabled'     => true,
                'config_name' => 'cmf_content',
            ),
        ));

``enabled``
...........

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

Enables/disables the CKeditor integration. ``auto`` will enable the integration
only when the IvoryCKEditorBundle is registered. ``false`` will always disable
the integration.

``config_name``
...............

**type**: ``string`` **default**: ``cmf_content``

The name of the IvoryCKEditorBundle config set to use for the editor.

.. _IvoryCKEditorBundle: https://github.com/egeloen/IvoryCKEditorBundle
