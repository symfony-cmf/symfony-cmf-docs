Configuration Reference
=======================

The TreeBrowserBundle can be configured under the ``cmf_tree_browser`` key in
your application configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/treebrowser`` namespace.

.. note::

    To use this bundle with the tree provided by SonataDoctrinePHPCRAdminBundle_,
    you do not need to provide any configuration here.

Configuration
-------------

.. _config-tree_browser-persistence:

``persistence``
~~~~~~~~~~~~~~~

``phpcr``
.........

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_tree_browser:
            persistence:
                phpcr:
                    enabled:      false
                    session_name: default

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/treebrowser">
                <persistence>
                    <phpcr
                        enabled="false"
                        session-name="default"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_tree_browser', [
            'persistence' => [
                'phpcr' => [
                    'enabled'      => false,
                    'session_name' => 'default',
                ],
            ],
        ]);


``enabled``
"""""""""""

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

``session_name``
""""""""""""""""

**type**: ``string`` **default**: ``default``

The name of the PHPCR connection to use.

.. _SonataDoctrinePHPCRAdminBundle: https://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
