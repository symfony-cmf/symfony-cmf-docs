Key Bundles
~~~~~~~~~~~

Most of the Symfony CMF bundles provide some functionality, they integrate a
library or they introduce a complete new concept.

* :doc:`block/index`

  * :doc:`block/introduction`
  * :doc:`block/types`
  * :doc:`block/create_your_own_blocks`
  * :doc:`block/cache`
  * :doc:`block/relation_to_sonata_block_bundle`
  * :doc:`block/configuration`

* :doc:`core/index`

  * :doc:`core/introduction`
  * :doc:`core/publish_workflow`
  * :doc:`core/templating`
  * :doc:`core/persistence`
  * :doc:`core/forms`
  * :doc:`core/configuration`

* :doc:`menu/index`

  * :doc:`menu/introduction`
  * :doc:`menu/menu_documents`
  * :doc:`menu/menu_factory`
  * :doc:`menu/menu_provider`
  * :doc:`menu/voters`
  * :doc:`menu/sonata_admin`
  * :doc:`menu/configuration`

* :doc:`routing/index`

  * :doc:`routing/introduction`
  * :doc:`routing/dynamic`
  * :doc:`routing/dynamic_customize`
  * :doc:`routing/configuration`

* :doc:`routing_auto/index`

  * :doc:`routing_auto/introduction`
  * :doc:`routing_auto/token_providers`
  * :doc:`routing_auto/conflict_resolvers`
  * :doc:`routing_auto/defunct_route_handlers`
  * :doc:`routing_auto/adapter`
  * :doc:`routing_auto/configuration`

* :doc:`seo/index`

  * :doc:`seo/introduction`
  * :doc:`seo/seo_aware`
  * :doc:`seo/extractors`
  * :doc:`seo/twig`
  * :doc:`seo/sitemap`
  * :doc:`seo/error_pages`
  * :doc:`seo/configuration`

* :doc:`tree_browser/index`

  * :doc:`tree_browser/introduction`
  * :doc:`tree_browser/configuration`

Admin Integrations
~~~~~~~~~~~~~~~~~~

Since version 2 of the CMF, we provide a separate bundle with admin implementations for the
SonataDoctrinePHPCRAdminBundle_.

* :doc:`sonata_phpcr_admin_integration/index`

Showcase Bundles
~~~~~~~~~~~~~~~~

Other bundles provide an example on how to use these key bundles. They all
show a different way of implementing a concept. You can use these bundles in
your project, but you can also get ideas from these bundles to create your own
bundle based on the key bundles.

* :doc:`content/index`

  * :doc:`content/introduction`
  * :doc:`content/configuration`
  * :doc:`content/exposing_content_via_rest`

Contributed Bundles
~~~~~~~~~~~~~~~~~~~

Besides that, the Symfony CMF team also maintains some third party bundles which
help with integrating PHPCR concepts into the Symfony Framework.

* :doc:`phpcr_odm/index`

  * :doc:`phpcr_odm/introduction`
  * :doc:`phpcr_odm/events`
  * :doc:`phpcr_odm/forms`
  * :doc:`phpcr_odm/fixtures_initializers`
  * :doc:`phpcr_odm/multilang`
  * :doc:`phpcr_odm/multiple_sessions`
  * :doc:`phpcr_odm/configuration`

Unmaintained Bundles
~~~~~~~~~~~~~~~~~~~~

The following bundles were created for the CMF, but to focus our efforts on the
core components, they are currently unmaintained.

* :doc:`create/index`

  * :doc:`create/introduction`
  * :doc:`create/other-editors`
  * :doc:`create/developing-hallo`
  * :doc:`create/configuration`

* :doc:`media/index`

  * :doc:`media/introduction`
  * :doc:`media/form_types`
  * :doc:`media/adapters/index`
  * :doc:`media/configuration`

* :doc:`search/index`

  * :doc:`search/introduction`
  * :doc:`search/configuration`

* :doc:`simple_cms/index`

  * :doc:`simple_cms/introduction`
  * :doc:`simple_cms/multilang`
  * :doc:`simple_cms/rendering`
  * :doc:`simple_cms/menus`
  * :doc:`simple_cms/extending_page_class`
  * :doc:`simple_cms/configuration`

.. _SonataDoctrinePHPCRAdminBundle: https://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
