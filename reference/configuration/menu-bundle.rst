Configuration
-------------

If you want to use default configurations, you do not need to change anything.
The values are:

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            menu_basepath:        /cms/menu
            document_manager_name: default
            admin_class:          ~
            document_class:       ~
            content_url_generator:  router
            route_name:           ~ # cmf routes are created by content instead of name
            content_basepath:     ~ # defaults to cmf_core.content_basepath
            allow_empty_items:    ~ # defaults to false
            voters:
                uri_prefix:       false # enable the UriPrefixVoter for current menu item
                content_identity: not set # enable the RequestContentIdentityVoter
                    content_key:  not set # override DynamicRouter::CONTENT_KEY
            use_sonata_admin:     auto # use true/false to force using / not using sonata admin
            multilang:            # the whole multilang section is optional
                use_sonata_admin:     auto # use true/false to force using / not using sonata admin
                admin_class:          ~
                document_class:       ~
                locales:              [] # if you use multilang, you have to define at least one locale

If you want to render the menu from twig, make sure you have not disabled twig
in the ``knp_menu`` configuration section.

If you have ``sonata-project/doctrine-phpcr-admin-bundle`` in your
``composer.json`` require section, the menu documents are exposed in the
SonataDoctrinePhpcrAdminBundle. For instructions on how to configure this
Bundle see :doc:`doctrine_phpcr_admin`.

By default, ``use_sonata_admin`` is automatically set based on whether
SonataDoctrinePhpcrAdminBundle is available, but you can explicitly disable it
to not have it even if sonata is enabled, or explicitly enable to get an error
if Sonata becomes unavailable.

By default, menu nodes that have neither the URI nor the routeName field set
and no route can be generated from the linked content are skipped by the
``ContentAwareFactory``. This also leads to their descendants not showing up.
If you want to generate menu items without a link instead, set the
``allow_empty_items`` parameter to true to make the menu items show up as
plain text instead.

