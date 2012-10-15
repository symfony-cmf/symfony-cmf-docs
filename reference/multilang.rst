SymfonyCmfMultilangContentBundle
================================

The `SymfonyCmfMultilangContentBundle <https://github.com/symfony-cmf/MultilangContentBundle#readme>`_
provides multi language capabilities for the Symfony2 content management framework.

This bundle provides translated versions of the documents in the other cmf
bundles and a helper service to render a language chooser.

.. index:: MultilangContentBundle, i18n

Features
--------

* Multilanguage version of the menu item document and content (TODO). (For routes, you
    create separate instances to place in the different locations in the tree)
* A service to render a language chooser to switch between the languages of the
    current page, based on the doctrine router.
* A controller to decide on the language based on the browser request, to redirect
    to the correct home page.


Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_multilang_content``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_multilang_content:
            document_manager_name:  default
            default_lang:         en
            lang_meta:
                name:                 []
            use_sonata_admin:     auto
            content_basepath:     ~

Usage
-----

Multilanguage documents
~~~~~~~~~~~~~~~~~~~~~~~

Just use them instead of the default documents when creating content. Loading
content detects multilanguage automatically.
Read more in the documentation of the `phpcr-odm`_

Language chooser
~~~~~~~~~~~~~~~~

You can render a language selection list in your templates using the language
selector controller. The idea is to have links to all language versions of the
current page.

::

    {% render "symfony_cmf_multilang_content.languageSelectorController:languagesAction"
            with {"id" : page.path, "languageUrls": languageUrls|default(false) } %}

For cmf pages, it is enough to specify the id (aka repository path) for the
content document. You do not need to specify the languageUrls, as the urls are
generated in the selector controller from the default language preferences
order.
But you can specify the languageUrls as parameter to the language chooser
i.e. to use a custom route name.

Language redirection controller
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This controller decides on the correct page to show for the browser preference
language. It is working with the RouteAwareInterface from the RoutingExtraBundle.

The idea is that you put a MultilangLanguageSelectRoute at the route where
language should be automatically detected (e.g. /)
Then you add normal routes having their locale set as children of that route.
This route will make the DoctrineRouter::generate() get all child routes.
``generate`` then chooses the route with the best available locale.
MultilangLanguageSelectRoute by default specifies the explicit controller

::

    symfony_cmf_multilang_content.languageSelectorController:defaultLanguageAction

.. _`phpcr-odm`: https://github.com/doctrine/phpcr-odm