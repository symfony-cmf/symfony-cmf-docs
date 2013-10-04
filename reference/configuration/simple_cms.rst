SimpleCmsBundle Configuration
=============================

The SimpleCmsBundle provide a simple CMS solution and can be configured under
the ``cmf_simple_cms`` key in your application configuration. When using
XML, you can use the ``http://cmf.symfony.com/schema/dic/simplecms`` namespace.

Configuration
-------------

.. _config-simple_cms-persistence:

persistence
~~~~~~~~~~~

phpcr
.....

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            persistence:
                phpcr:
                    enabled: false
                    basepath: /cms/simple
                    manager_registry: doctrine_phpcr
                    manager: ~
                    document_class: Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page
                    use_sonata_admin: auto
                    sonata_admin:
                        sort: false

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/simplecms">
                <persistence>
                    <phpcr
                        enabled="false"
                        basepath="/cms/simple"
                        manager-registery="doctrine_phpcr"
                        manager-name="null"
                        document-class="Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page"
                        use-sonata-admin="auto"
                    >
                        <sonata-admin sort="false" />
                    </phpcr>
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_simple_cms', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled' => false,
                    'basepath' => '/cms/simple',
                    'manager_registry' => 'doctrine_phpcr',
                    'manager_name' => null,
                    'document_class' => 'Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page',
                    'use_sonata_admin' => 'auto',
                    'sonata_admin' => array(
                        'sort' => false,
                    ),
                ),
            ),
        ));


enabled
"""""""

.. include:: partials/persistence_phpcr_enabled.rst.inc

basepath
""""""""

**type**: ``string`` **default**: ``/cms/simple``

The basepath for CMS documents in the PHPCR tree.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/simple``.

manager_registry
""""""""""""""""

**type**: ``string`` **default**: ``doctrine_phpcr``

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.manager_registry``.

manager_name
""""""""""""

.. include:: partials/persistence_phpcr_manager_name.rst.inc

document_class
""""""""""""""

**type**: ``string`` **default**: ``'Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page'``

The class for the pages, used by sonata admin.

use_sonata_admin
""""""""""""""""

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the admin classes for SimpleCmsBundle pages are activated. If set
to ``auto``, the admin services are activated only if the
SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to the value
of ``cmf_core.persistence.phpcr.use_sonata_admin``.

sonata_admin.sort
"""""""""""""""""

**type**: ``enum`` **valid values**: ``false|asc|desc`` **default**: ``false``

If set to ``asc`` or ``desc``, sonata admin will ensure that the pages are
sorted ascending or descending when storing in PHPCR. Sorting takes publication
date first, then creation date.

.. _config-simple_cms-use_menu:

use_menu
~~~~~~~~

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If set to ``auto``, menu integration is loaded if the CmfMenuBundle is
registered with the kernel. If set to ``true``, the integration is always
loaded, leading to an error should CmfMenuBundle not be available. Setting it
to ``false`` does not load the menu integration even if the CmfMenuBundle is
registered.

The menu integration loads a menu provider that provides the tree of Pages as
menu. The name of that menu is the name used in the ``basepath``.

.. note::

    The default ``Page`` model class implements on an interface from
    KnpMenuBundle, meaning at least that bundle must be in the code base. This
    is reflected by the composer.json requiring it.

Configure integration with CmfMenuBundle.

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            use_menu: auto

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/simplecms"
                use-menu="auto"
            />
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_simple_cms', array(
            'use_menu' => 'auto',
        ));

.. _config-simple-cms-routing:

routing
~~~~~~~

This configures how pages should be rendered. The simple cms uses its own
instance of the ``DynamicRouter``. The options here are the same as described
in :ref:`routing configuration <reference-config-routing-dynamic>`.

Pages that are loaded from the ``cmf_simple_cms.persistence.phpcr.basepath``
need to be configured here. Pages loaded from the
``cmf_routing.persistence.phpcr.basepath`` must be configured in the
CmfRoutingBundle configuration.

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            routing:
                controller_by_alias: []
                controller_by_class: []
                templates_by_class:
                  Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page: CmfSimpleCmsBundle:Page:index.html.twig
                generic_controller: cmf_content.controller:indexAction
                content_repository_id: cmf_routing.content_repository
                uri_filter_regexp: ~

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/simplecms">
                <routing xmlns="http://cmf.symfony.com/schema/dic/simplecms">
                    <controller-by-alias></controller-by-alias>
                    <controller-by-class></controller-by-class>
                    <template-by-class alias="Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page">CmfSimpleCmsBundle:Page:index.html.twig</template-by-class>
                    <generic-controller>cmf_content.controller:indexAction</generic-controller>
                    <content-repository-id>cmf_routing.content_repository</content-repository-id>
                    <uri-filter-regexp>null</uri-filter-regexp>
                </routing>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_simple_cms', array(
            'routing' => array(
                'controller_by_alias' => array(),
                'controller_by_class' => array(),
                'templates_by_class' => array(
                    'Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page' => 'CmfSimpleCmsBundle:Page:index.html.twig',
                ),
                'generic_controller' => 'cmf_content.controller:indexAction',
                'content_repository_id' => 'cmf_routing.content_repository',
                'uri_filter_regexp' => null,
            ),
        ));

.. _config-simple_cms-multilang:

multilang
~~~~~~~~~

Multilanguage is activated if the ``locales`` option is configured (either in
SimpleCmsBundle or in CoreBundle).

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            multilang:
                locales: [en, fr]

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/simplecms">
                <multilang>
                    <locales>en</locales>
                    <locales>fr</locales>
                </multilang>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_simple_cms', array(
            'multilang' => array(
                'locales' => array(
                    'en',
                    'fr',
                ),
            ),
        ));

locales
.......

**type**: ``array`` **default**: ``null``

This define languages that can be used.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``cmf_core.multilang.locales``.
