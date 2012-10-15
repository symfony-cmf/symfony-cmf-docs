SymfonyCmfSearchBundle
======================

The `SymfonyCmfSearchBundle <https://github.com/symfony-cmf/SearchBundle#readme>`_
provides integration with LiipSearchBundle to handle a site wide search.

.. index:: SearchBundle

Dependencies
------------

* `LiipSearchBundle <https://github.com/liip/LiipSearchBundle#readme>`_

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_search``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_search:
            document_manager_name:  default
