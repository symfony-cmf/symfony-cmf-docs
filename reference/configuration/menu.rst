MenuBundle configuration
========================

The MenuBundle provides a persistent menu system based on the KnpMenuBundle.
It can be configured under the ``cmf_menu`` key in your application
configuration. When using XML you should use the
``http://cmf.symfony.com/schema/dic/content`` namespace.

Configuration
-------------

.. _config-menu-persistence:

persistence
~~~~~~~~~~~

This defines the persistence driver and associated classes. The default
persistence configuration has the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            persistence:
                phpcr:
                    enabled:              false
                    menu_basepath:        /cms/menu
                    content_basepath:     ~
                    manager_name:         ~
                    menu_document_class:  Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\Menu
                    node_document_class:  Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNode
                    use_sonata_admin:     ~ # One of true; false; "auto"
                    menu_admin_class:     Symfony\Cmf\Bundle\MenuBundle\Admin\MenuAdmin
                    node_admin_class:     Symfony\Cmf\Bundle\MenuBundle\Admin\MenuNodeAdmin
                    admin_recursive_breadcrumbs:  true


    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <persistence>
                    <phpcr
                        enabled="false"
                        menu-basepath="/cms/menu"
                        content-basepath="null"
                        manager-name="null"
                        menu-document-class="null"
                        node-document-class="null"
                        use-sonata-admin="auto"
                        menu-admin-class="null"
                        node-admin-class="null"
                        admin-recursive-breadcrumbs="true"
                    />
                </persistence>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_menu', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled' => false,
                    'menu_basepath' => '/cms/menu',
                    'content_basepath' => null,
                    'manager_name' => null,
                    'menu_document_class' => null,
                    'node_document_class' => null,
                    'use_sonata_admin' => 'auto',
                    'menu_admin_class' => null,
                    'node_admin_class' => null,
                    'admin_recursive_breadcrumbs' => true,
                ),
            ),
        ));

enabled
"""""""

.. include:: partials/persistence_phpcr_enabled.rst.inc

manager_name
""""""""""""

.. include:: partials/persistence_phpcr_manager_name.rst.inc

menu_basepath
"""""""""""""

**type**: ``string`` **default**: ``/cms/menu``

Specifies the path in the PHPCR-ODM document hierarchy under which the menu
documents can be found.

* This is the default location used by the 
  :doc:`MenuProvider <../../bundles/menu/menu_factory>` to locate menus.
* This is the default location for newly created menus in the Sonata admin
  class.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/menu``

content_basepath
""""""""""""""""

**type**: ``string`` **default**: ``/cms/content``

Specifies the path in the PHPCR-ODM document hierarchy under which the
content documents can be found. This is used by the admin class to pre-select
the content branch of the document hierarchy in forms.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/content``

menu_document
"""""""""""""

**type**: ``string`` **default**:
``Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\Menu``

Specifies the document class which should represent an entire menu.

This setting is used by the admin class.

node_document
"""""""""""""

**type**: ``string`` **default**:
``Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNode``

Specifies the document class which should represent a single menu node.

This setting is used by the admin class.

use_sonata_admin
""""""""""""""""

.. include:: partials/persistence_phpcr_sonata_admin_enabled.rst.inc

menu_admin_class
""""""""""""""""

**type**: ``string`` **default**:
``Symfony\Cmf\Bundle\MenuBundle\Admin\MenuAdmin``

The Sonata admin class to use for the menu.

node_admin_class
""""""""""""""""

**type**: ``string`` **default**:
``Symfony\Cmf\Bundle\MenuBundle\Admin\MenuNodeAdmin``

The Sonata admin class to use for the menu node.

admin_recursive_breadcrumbs
"""""""""""""""""""""""""""

**type**: ``boolean`` **default**: ``true``

When editing a node, this setting will cause the Sonata admin breadcrumb to include ancestors of the node being edited.

Voter
-----

**type**: ``array``

The ``voters`` section enables you to enable and configure *pre-defined* :doc:`voters <../../bundles/menu/voters>`.

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            # ... 
            voters:
                content_identity:
                    content_key:          ~
                uri_prefix:           false

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <voter uri-prefix="false">
                    <content-identity content-key="null" />
                </voter>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_menu', array(
            'persistence' => array(
                'voters' => array(
                    'content_identity' => array(
                        'content_key' => null,
                    ),
                    'uri_prefix' => false,
                ),
            ),
        ));

content_identity
~~~~~~~~~~~~~~~~

**type**: ``array|boolean``

Enable the :ref:`bundles_menu_voters_request_identity_voter`.

Configuring this key is enough to enable it.

content_key
"""""""""""

**type**: ``string`` **default**: ``contentDocument``

The name of the parameter containing the content in the request.

.. note::

    If you are using the RoutingBundle, you do not need to set this as it will default to
    ``DynamicRouter::CONTENT_KEY``. If however you do not use the RoutingBundle, you will have to specify a key.

uri_prefix
~~~~~~~~~~

**type**: ``boolean``

Enable the :ref:`bundles_menu_voters_uri_prefix_voter`.
