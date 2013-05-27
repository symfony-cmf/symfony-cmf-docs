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

The configuration key for this bundle is ``symfony_cmf_search``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_search:
            document_manager_name:  default
            translation_strategy: child # can also be set to an empty string or attribute
            translation_strategy: attribute
            search_path: /cms/content
            search_fields:
                title: title
                summary: body

.. _`SearchBundle`: https://github.com/symfony-cmf/SearchBundle#readme
.. _`LiipSearchBundle`: https://github.com/liip/LiipSearchBundle
.. _`LiipSearchBundle`: https://github.com/liip/LiipSearchBundle
