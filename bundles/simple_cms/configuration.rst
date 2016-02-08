Configuration Reference
=======================

The SimpleCmsBundle can be configured under the ``cmf_simple_cms`` key in your
application configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/simplecms`` namespace.

Configuration
-------------

.. _config-simple_cms-persistence:

persistence
~~~~~~~~~~~

``phpcr``
.........

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            persistence:
                phpcr:
                    enabled:          false
                    basepath:         /cms/simple
                    manager_registry: doctrine_phpcr
                    manager_name:     ~
                    document_class:   Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page
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
                    'enabled'          => false,
                    'basepath'         => '/cms/simple',
                    'manager_registry' => 'doctrine_phpcr',
                    'manager_name'     => null,
                    'document_class'   => 'Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page',
                    'use_sonata_admin' => 'auto',
                    'sonata_admin' => array(
                        'sort' => false,
                    ),
                ),
            ),
        ));


``enabled``
"""""""""""

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

``basepath``
""""""""""""

**type**: ``string`` **default**: ``/cms/simple``

The basepath for CMS documents in the PHPCR tree.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/simple``.

``manager_registry``
""""""""""""""""""""

**type**: ``string`` **default**: ``doctrine_phpcr``

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.manager_registry``.

``manager_name``
""""""""""""""""

.. include:: ../_partials/persistence_phpcr_manager_name.rst.inc

``document_class``
""""""""""""""""""

**type**: ``string`` **default**: ``'Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page'``

The class for the pages, used by sonata admin.

``use_sonata_admin``
""""""""""""""""""""

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the admin classes for SimpleCmsBundle pages are activated. If set
to ``auto``, the admin services are activated only if the
SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to the value
of ``cmf_core.persistence.phpcr.use_sonata_admin``.

``sonata_admin.sort``
"""""""""""""""""""""

**type**: ``enum`` **valid values**: ``false|asc|desc`` **default**: ``false``

If set to ``asc`` or ``desc``, sonata admin will ensure that the pages are
sorted ascending or descending when storing in PHPCR. Sorting takes publication
date first, then creation date.

.. _config-simple_cms-use_menu:

``use_menu``
~~~~~~~~~~~~

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

``routing``
~~~~~~~~~~~

.. versionadded:: 1.1
    Since SimpleCmsBundle 1.1, this configuration is done directly on the
    :ref:`RoutingBundle <reference-config-routing-dynamic>`.

``multilang``
~~~~~~~~~~~~~

.. versionadded:: 1.1
    Since SimpleCmsBundle 1.1, this configuration is done directly on the
    :ref:`RoutingBundle <reference-config-routing-locales>`.

.. include:: ../_partials/ivory_ckeditor.rst.inc
