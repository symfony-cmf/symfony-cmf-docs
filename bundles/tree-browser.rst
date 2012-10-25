TreeBrowserBundle
=================

The `TreeBrowserBundle <https://github.com/symfony-cmf/TreeBrowserBundle#readme>`_
provides integration with :doc:`/bundles/tree` to provide a tree navigation on top of a PHPCR repository.

.. index:: TreeBrowserBundle

Dependencies
------------

* `TreeBundle <https://github.com/symfony-cmf/TreeBundle#readme>`_

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_tree_browser``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_tree_browser:
            session:  default
