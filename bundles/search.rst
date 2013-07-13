.. index::
    single: Search; Bundles
    single: SearchBundle

The SearchBundle
================

The `SearchBundle`_ provides integration with `LiipSearchBundle`_ to provide a
site wide search.

.. index:: SearchBundle

Dependencies
------------

* `LiipSearchBundle`_

Configuration
-------------

The configuration key for this bundle is ``cmf_search``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_search:
            document_manager_name:  default
            translation_strategy: child # can also be set to an empty string or attribute
            translation_strategy: attribute
            search_path: /cms/content
            search_fields:
                title: title
                summary: body

    .. code-block:: xml

        <!-- app/config/config.xml -->
        
        <!-- translation-strategy: can also be set to an empty string or attribute -->
        <config xmlns="http://cmf.symfony.com/schema/dic/search"
            document-manager-name="default"
            translation-strategy="child"
            search-path="/cms/content">
                <search-fields>
                    <title>title</title>
                    <summary>body</summary>
                </search-fields>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_search', array(
            'document_manager_name' => 'default',
            'translation_strategy'  => 'child', // can also be set to an empty string or attribute
            'translation_strategy'  => 'attribute',
            'search_path'           => '/cms/content',
            'search_fields'         => array(
                'title'   => 'title',
                'summary' => 'body',
            ),
        ));

.. _`SearchBundle`: https://github.com/symfony-cmf/SearchBundle#readme
.. _`LiipSearchBundle`: https://github.com/liip/LiipSearchBundle
.. _`LiipSearchBundle`: https://github.com/liip/LiipSearchBundle
