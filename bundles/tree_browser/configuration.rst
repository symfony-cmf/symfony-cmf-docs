Configuration Reference
=======================

The TreeBrowserBundle can be configured under the ``cmf_tree_browser`` key in
your application configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/treebrowser`` namespace.

Configuration
-------------

.. _config-tree_browser-icons:

``icons``
~~~~~~~~~

**prototype**: ``scalar``

A mapping of interfaces/classes to icons. These icons will be shown in the tree
whenever an instance of this interface/class is shown.

The icon can be either a CSS class (e.g. to use an icon font) or a URI to an
image. The value is considered to be an URI when it contains a ``.`` or ``/``
character.

.. configuration-block::

    .. code-block:: yaml

        cmf_tree_browser:
            icons:
                # URI to an image
                Symfony\Cmf\Component\Routing\RouteObjectInterface: '/img/route-icon.png'

                # CSS classes
                Symfony\Cmf\Bundle\SeoBundle\Model\SeoMetadata: 'fa fa-search'

    .. code-block:: xml

        <?xml version="2.0" encoding="UTF-8"?>
        <container xmlns="http://symfony.com/dic/schema/services"
            xmlns:xsd="http://www.w3.org/2001/XMLSchema-instance"
            xsd:schemaLocation="http://symfony.com/dic/schema/services http://symfony.com/dic/schema/services/services-1.0.xsd
                http://cmf.symfony.com/dic/schema/tree_browser http://cmf.symfony.com/dic/schema/tree_browser/tree_browser-1.0.xsd"
        >

            <config xmlns="http://cmf.symfony.com/dic/schema/tree_browser">

                <!-- URI to an image -->
                <icon class="Symfony\Cmf\Component\Routing\RouteObjectInterface">/img/route-icon.png</icon>

                <!-- CSS classes -->
                <icon class="Symfony\Cmf\Bundle\SeoBundle\Model\SeoMetadata">fa fa-search</icon>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_tree_browser', [
            'icons' => [
                // URI to an image
                'Symfony\Cmf\Component\Routing\RouteObjectInterface' => '/img/route-icon.png',

                // CSS classes
                'Symfony\Cmf\Bundle\SeoBundle\Model\SeoMetadata' => 'fa fa-search',
            ],
        ]);

.. _SonataDoctrinePHPCRAdminBundle: http://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
