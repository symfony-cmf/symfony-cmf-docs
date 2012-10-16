SymfonyCmfSimpleCmsBundle
=========================

The `SymfonyCmfSimpleCmsBundle <https://github.com/symfony-cmf/SimpleCmsBundle#readme>`_
provides a simplistic CMS on top of the CMF components and bundles.

While the core CMF components focus on flexibility, the simple CMS trades
away some of that flexibility in favor of simplicity.

.. index:: SimpleCmsBundle, i18n

Dependencies
------------

As specified in the bundle ``composer.json`` this bundle depends on most CMF bundles.

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_simple_cms``


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_simple_cms:
            use_sonata_admin:     true
            use_menu:             true
            document_class:       Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page
            generic_controller:   symfony_cmf_content.controller:indexAction
            basepath:             /cms/simple
            routing:
                content_repository_id:  symfony_cmf_routing_extra.content_repository
                controllers_by_class:

                    # Prototype
                    alias:                []
                templates_by_class:

                    # Prototype
                    alias:                []
                multilang:
                    locales:              []

.. Tip::

    If you have the sonata phpcr-odm admin bundle enabled but do *NOT* want to
    show the default admin provided by this bundle, you can add the following
    to your configuration

    .. configuration-block::

        .. code-block:: yaml

            symfony_cmf_simple_cms:
                use_sonata_admin: false

Multi-language support
----------------------

In multi-language-mode the Bundle will automatically use the
``Symfony\Cmf\Bundle\SimpleCmsBundle\Document\MultilangPage`` as the ``document_class``
unless a different class is configured explicitly.

This class will by default
prefix all routes with ``/{_locale}``. This behavior can be disabled by setting the
second parameter in the constructor of the model to false.

Furthermore the routing layer will be configured
to use ``Symfony\Cmf\Bundle\SimpleCmsBundle\Document\MultilangRouteRepository`` which will
ensure that even with the locale prefix the right content node will be found. Furthermore
it will automatically add a ``_locale`` requirement listing the current available locales
for the matched route.

.. Note::

    Since SimpleCmsBundle only provides a single tree structure, all nodes will have the same
    node name for all languages. So a url ``http://foo.com/en/bar`` for english content will
    look like ``http://foo.com/de/bar`` for german content. At times it might be most feasible
    to use integers as the node names and simple append the title of the node in the given locale
    as an anchor. So for example ``http://foo.com/de/1#my title`` and ``http://foo.com/de/1#mein titel``.