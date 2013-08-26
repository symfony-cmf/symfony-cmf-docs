.. index::
    single: Content; Bundles
    single: ContentBundle

ContentBundle
=============

This bundle provides a document for static content and the controller to
render it.

For an introduction see the ":doc:`../book/static_content`" article in the
"Book" section.

.. index:: ContentBundle

Configuration
-------------

The configuration key for this bundle is ``cmf_content``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_content:
            admin_class:          ~
            document_class:       ~
            default_template:     ~
            content_basepath:     /cms/content
            static_basepath:      /cms/content/static
            use_sonata_admin:     auto
            multilang:                # the whole multilang section is optional
                admin_class:          ~
                document_class:       ~
                use_sonata_admin:     auto
                locales:              [] # if you use multilang, you have to define at least one locale

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <config xmlns="http://cmf.symfony.com/schema/dic/content"
            admin-class=""
            document-class=""
            default-template=""
            content-basepath="/cms/content"
            static-basepath="/cms/content/static"
            use-sonata-admin="auto">

            <!-- the whole multilang section is optional -->
            <multilang
                admin-class=""
                document-class=""
                use-sonata-admin="auto">
                <locale><!-- if you use multlang, you have to define at least one locale --></locale>
            </multilang>
        </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_content', array(
            'admin_class'      => null,
            'document_class'   => null,
            'default_template' => null,
            'content_basepath' => '/cms/content',
            'static_basepath'  => '/cms/content/static',
            'use_sonata_admin' => 'auto',
            // the whole multilang section is optional
            'multilang'        => array(
                'admin_class'      => null,
                'document_class'   => null,
                'use_sonata_admin' => 'auto',
                'locales'          => array(
                    // ... if you use multilang, you have to define at least one locale
                ),
            ),
        ));
