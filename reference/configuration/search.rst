SearchBundle Configuration
==========================

The SearchBundle provides integration with `LiipSearchBundle`_ to provide a
site wide search and can be configured under the ``cmf_search`` key in your
application configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/search`` namespace.

Configuration
-------------

.. _config-search-persistence:

persistence
~~~~~~~~~~~

phpcr
.....

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_search:
            show_paging:          false
            persistence:
                phpcr:
                    enabled:              false
                    search_basepath:      /cms/content
                    manager_registry:     doctrine_phpcr
                    manager_name:         ~
                    translation_strategy: ~
                    search_fields:        []

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr
                        enabled="false"
                        search-basepath="/cms/content"
                        manager-registery="doctrine_phpcr"
                        manager-name="null"
                        translation-strategy="null"
                    >
                        <search-field>
                            <title>title</title>
                            <summary>body</summary>
                        </search-field>
                    </phpcr>
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_search', array(
            'enabled' => false,
            'search_basepath'           => '/cms/content',
            'manager_registry' => 'doctrine_phpcr',
            'manager_name' => null,
            'translation_strategy'  => null
            'search_fields'         => array(
                'title'   => 'title',
                'summary' => 'body',
            ),
        ));

enabled
"""""""

.. include:: partials/persistence_phpcr_enabled.rst.inc

search_basepath
"""""""""""""""

**type**: ``string`` **default**: ``/cms/content``

The basepath for CMS documents in the PHPCR tree.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/content``.

manager_registry
""""""""""""""""

**type**: ``string`` **default**: ``doctrine_phpcr``

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.manager_registry``.

manager_name
""""""""""""

.. include:: partials/persistence_phpcr_manager_name.rst.inc

translation_strategy
""""""""""""""""""""

**type**: ``string`` **default**: ``null``

The translation strategy used in the documents that are searched. Can either be ``null``,
``child`` or ``attribute``.

search_fields
"""""""""""""

**type**: ``array``  **default**: ``array()``

The PHPCR node properties that should be read and passed to the template. The key is the name
of the PHPCR node property, the value is the variable name used to pass the data to the
template.

.. configuration-block::

    .. code-block:: yaml

        search_fields:
            title: title
            summary: body

    .. code-block:: xml

        <search-field>
            <title>title</title>
            <summary>body</summary>
        </search-field>

    .. code-block:: php

        $container->loadFromExtension('cmf_search', array(
            'search_fields'         => array(
                'title'   => 'title',
                'summary' => 'body',
            ),
        ));

.. _`LiipSearchBundle`: https://github.com/liip/LiipSearchBundle
