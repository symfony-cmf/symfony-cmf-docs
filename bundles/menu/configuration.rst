Configuration Reference
=======================

The MenuBundle can be configured under the ``cmf_menu`` key in your application
configuration. When using XML you should use the
``http://cmf.symfony.com/schema/dic/menu`` namespace.

Configuration
-------------

.. _config-menu-persistence:

``persistence``
~~~~~~~~~~~~~~~

This defines the persistence driver and associated classes. The default
persistence configuration has the following configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_menu:
            persistence:
                phpcr:
                    enabled:                     false
                    menu_basepath:               /cms/menu
                    content_basepath:            /cms/content
                    manager_name:                ~
                    prefetch:                    10
                    menu_document_class:         Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\Menu
                    node_document_class:         Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNode

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <persistence>
                    <phpcr
                        enabled="false"
                        menu-basepath="/cms/menu"
                        content-basepath="/cms/content"
                        manager-name="null"
                        prefetch="10"
                        menu-document-class="Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\Menu"
                        node-document-class="Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNode"
                    />
                </persistence>
            </config>
        </container>

    .. code-block:: php

        //  app/config/config.php
        $container->loadFromExtension('cmf_menu', [
            'persistence' => [
                'phpcr' => [
                    'enabled'                     => false,
                    'menu_basepath'               => '/cms/menu',
                    'content_basepath'            => '/cms/content',
                    'manager_name'                => null,
                    'prefetch'                    => 10,
                    'menu_document_class'         => \Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\Menu::class,
                    'node_document_class'         => \Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNode::class,
                ],
            ],
        ]);

``enabled``
"""""""""""

.. include:: ../_partials/persistence_phpcr_enabled.rst.inc

``menu_basepath``
"""""""""""""""""

**type**: ``string`` **default**: ``/cms/menu``

Specifies the path in the PHPCR-ODM document hierarchy under which the menu
documents can be found.

* This is the default location used by the
  :doc:`MenuProvider <../menu/menu_factory>` to locate menus.
* This is the default location for newly created menus in the Sonata admin
  class.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/menu``

``content_basepath``
""""""""""""""""""""

**type**: ``string`` **default**: ``/cms/content``

Specifies the path in the PHPCR-ODM document hierarchy under which the
content documents can be found. This is used by the sonata admin integration to
know which subtree to show when selecting content for menu nodes.

If the :doc:`CoreBundle <../core/introduction>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/content``

.. versionadded:: 1.1

    The pre-fetch functionality was added in MenuBundle 1.1.

``prefetch``
""""""""""""

**type**: ``integer`` **default**: ``10``

When rendering a menu, the whole menu tree needs to be loaded. To reduce the
number of database requests that PHPCR needs to make, this setting makes the
tree loader pre-fetch all menu nodes in one call.

``10`` should be enough for most cases, if you have deeper menu structures you
might want to increase this.

To disable menu pre-fetch completely, set this option to ``0``.

``manager_name``
""""""""""""""""

.. include:: ../_partials/persistence_phpcr_manager_name.rst.inc

``menu_document_class``
"""""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\Menu``

Specifies the document class which should represent an entire menu.

This setting is used by the admin class.

``node_document_class``
"""""""""""""""""""""""

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\MenuBundle\Doctrine\Phpcr\MenuNode``

Specifies the document class which should represent a single menu node.

This setting is used by the admin class.

content_url_generator
~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 1.2
    This option was introduced in version 1.2.0. Prior to 1.2, this option is
    not available and the default service ``router`` is hardcoded.

**type**: ``string`` **default**: ``router``

With this option, you can change what router should be used for generating
URLs from menu nodes of type "content".

allow_empty_items
~~~~~~~~~~~~~~~~~

**type**: ``boolean`` **default**: ``false``

Whether menu nodes without URL should be hidden or rendered as text without a
link.

Voter
-----

**type**: ``array``

The ``voters`` section enables you to enable and configure *pre-defined*
:doc:`voters <../menu/voters>`.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_menu:
            # ...
            voters:
                content_identity:
                    content_key: ~
                uri_prefix: false

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <voter uri-prefix="false">
                    <content-identity content-key="null" />
                </voter>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_menu', [
            'persistence' => [
                'voters' => [
                    'content_identity' => [
                        'content_key' => null,
                    ],
                    'uri_prefix' => false,
                ],
            ],
        ]);

``content_identity``
~~~~~~~~~~~~~~~~~~~~

**type**: ``array|boolean``

Enable the :ref:`bundles_menu_voters_request_identity_voter`.

Configuring this key is enough to enable it.

``content_key``
"""""""""""""""

**type**: ``string`` **default**: ``contentDocument``

The name of the parameter containing the content in the request.

.. note::

    If you are using the RoutingBundle, you do not need to set this as it will default to
    ``DynamicRouter::CONTENT_KEY``. If however you do not use the RoutingBundle, you will have to specify a key.

``uri_prefix``
~~~~~~~~~~~~~~

**type**: ``boolean``

Enable the :ref:`bundles_menu_voters_uri_prefix_voter`.

``publish_workflow``
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 1.1
    The ``publish_workflow`` option was introduced in CmfMenuBundle 1.1.

This configures if the menu content voter for the publish workflow should be
disabled, by default it is enabled if the :doc:`CoreBundle <../core/introduction>`
is registered, and the ``cmf_core.publish_workflow`` is enabled.

For more information refer to the
:doc:`publish workflow documentation <../core/publish_workflow>`.

To disable the menu content voter, use:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_core:
            publish_workflow:
                enabled: false

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <publish-workflow
                    enabled="false"
                />
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', [
            'publish_workflow' => [
                'enabled' => false,
            ],
        ]);
