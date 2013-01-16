ContentBundle
=============

This bundle provides a document for static content and the controller to render it.

For more information see the :doc:`../getting-started/content` page on the
"Getting started" section, or the documentation of the `ContentBundle <https://github.com/symfony-cmf/ContentBundle#readme>`_
on the project's GitHub page.

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
            multilang:                # the whole multilang section is optionnal
                admin_class:          ~
                document_class:       ~
                use_sonata_admin:     auto
                locales:              [] # if you use multilang, you have to define at least one locale
