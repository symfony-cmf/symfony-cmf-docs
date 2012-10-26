ContentBundle
=============

This bundle provides a document for static content and the controller to render it.

For more information for now see the documentation of the `ContentBundle <https://github.com/symfony-cmf/ContentBundle#readme>`_

.. index:: ContentBundle

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_content``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_content:
            admin_class:          ~
            document_class:       ~
            default_template:     ~
            content_basepath:     /cms/content
            static_basepath:      /cms/content/static
            use_sonata_admin:     auto
            multilang:
                admin_class:          ~
                document_class:       ~
                use_sonata_admin:     auto
                locales:              []
