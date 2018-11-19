Configuration Reference
=======================

The ContentBundle can be configured under the ``cmf_content`` key in your
application configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/content`` namespace.

Configuration
-------------

``default_template``
~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``null``

This defines the template to use when rendering the content if the routing does
not specify the template. ``{_format}`` and ``{_locale}`` are replaced with the
request format and the current locale.

.. _config-content-persistence:

``persistence``
~~~~~~~~~~~~~~~

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_content:
            persistence:
                phpcr:
                    enabled:          false
                    content_basepath: /cms/content
                    manager_name: null

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/content">
                <persistence>
                    <phpcr
                        enabled="false"
                        content-basepath="/cms/content"
                        manager-name="null"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_content', [
            'persistence' => [
                'phpcr' => [
                    'enabled'          => false,
                    'content_basepath' => '/cms/content',
                    'manager_name'     => null,
                ],
            ],
        ]);

``enabled``
...........

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

``content_basepath``
....................

**type**: ``string`` **default**: ``/cms/content``

The basepath for content documents in the PHPCR tree.

``manager_name``
................

**type**: ``string`` **default**: ``null``

The document manager that holds StaticContent documents. If not set, the
default manager is used.
