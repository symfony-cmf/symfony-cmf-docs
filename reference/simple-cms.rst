SymfonyCmfSimpleCmsBundle
=========================

The `SymfonyCmfSimpleCmsBundle <https://github.com/symfony-cmf/SimpleCmsBundle#readme>`_
provides a simplistic CMS on top of the CMF components and bundles.

While the core CMF components focus on flexibility, the simple CMS trades
away some of that flexibility in favor of simplicity.

.. index:: SimpleCmsBundle

Dependencies
------------

As specified in the bundle ``composer.json`` this bundle depends on most CMF bundles.

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_simple_cms``


.. Tip::

    If you have the sonata phpcr-odm admin bundle enabled but do *NOT* want to
    show the default admin provided by this bundle, you can add the following
    to your configuration

    .. configuration-block::

        .. code-block:: yaml

            symfony_cmf_simple_cms:
                use_sonata_admin: false
