SymfonyCmfContentBundle
=======================
This bundle provides a document for static content and the controller to render it.

For more information for now see the documentation of the `SymfonyCmfContentBundle <https://github.com/symfony-cmf/ContenteBundle#readme>`_

.. index:: ContentBundle

Configuration
-------------

The configuration key for this bundle is ``symfony_cmf_content``

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_content:
            document_class: Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent
            default_template: SymfonyCmfContentBundle:StaticContent:index.html.twig
            content_basepath: /cms/content # used by several Bundles as the root path to store content
            static_basepath: /cms/content/static # used by several Bundles as the root path to store static content
