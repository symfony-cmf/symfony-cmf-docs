MenuBundle
==========

    This integration becomes available once the :doc:`CmfMenuBundle
    <../menu/introduction>` is installed.

The menu admin tools are the admins for menus and menu nodes, as well as an
admin extension to edit which menu nodes point to a content from within the
content page.

The menu admin uses the ``cmf_menu.persistence.phpcr.menu_basepath`` setting to
determine where to add menus. The menu node admin additionally uses
``cmf_menu.persistence.phpcr.content_basepath`` to show the tree used to select
content for a menu node.

There are some general settings for the menu admin:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                menu:
                    recursive_breadcrumbs: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="2.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsd="http://www.w3.org/2001/XMLSchema-instance"
            xsd:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://cmf.symfony.com/schema/dic/sonata_admin_integration http://cmf.symfony.com/schema/dic/sonata_admin_integration/sonata_admin_integration.xsd"
        >

            <config xmlns="http://cmf.symfony.com/schema/dic/sonata_admin_integration">
                <bundles>
                    <menu recursive-breadcrumbs="true"/>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'menu' => [
                    'recursive_breadcrumbs' => true,
                ],
            ],
        ]);

recursive_breadcrumbs
~~~~~~~~~~~~~~~~~~~~~

**type**: ``boolean`` **default**: ``true``.

If this is set to true, the breadcrumb in the menu item admin shows the
ancestor menu items in addition to the link for home and the menu containing
the current menu item.

MenuNodeReferrersInterface Sonata Admin Extension
-------------------------------------------------

The menu admin integration provides an extension to edit referring menu nodes
for content that implements the ``MenuNodeReferrersInterface``.

The extension can be separately disabled and you can define the form group and
tab to be used by the field this extension adds:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                menu:
                    extensions:
                        menu_node_referrers:
                            enabled: true
                            form_group: form.group_menus
                            form_tab: form.tab_menu

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="2.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsd="http://www.w3.org/2001/XMLSchema-instance"
            xsd:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://cmf.symfony.com/schema/dic/sonata_admin_integration http://cmf.symfony.com/schema/dic/sonata_admin_integration/sonata_admin_integration.xsd"
        >

            <config xmlns="http://cmf.symfony.com/schema/dic/sonata_admin_integration">
                <bundles>
                    <menu>
                        <extensions>
                            <menu-node-referrers
                                enabled="true"
                                form-group="form.group_menus"
                                form-tab="form.tab_menu"
                             />
                        </extensions>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'menu' => [
                    'extensions' => [
                        'menu_node_referrers' => [
                            'enabled' => true,
                            'form_group' => 'form.group_menus',
                            'form_tag' => 'form.tab_menu',
                        ],
                    ],
                ],
            ],
        ]);

``enabled``
~~~~~~~~~~~

**type**: ``bool`` **default**: ``true``

If ``false``, the extension is not loaded at all to save resources.

``form_group``
~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``form.group_menus``

.. include:: ../_partials/sonata_admin_form_group.rst.inc

``form_tab``
~~~~~~~~~~~~

**type**: ``string`` **default**: ``form.tab_menu``

.. include:: ../_partials/sonata_admin_form_tab.rst.inc

Using the extension
~~~~~~~~~~~~~~~~~~~

To use the extension in your admin classes, define the extension in the
``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_sonata_phpcr_admin_integration.menu.extension.menu_node_referrers:
                    implements:
                        - Symfony\Cmf\Bundle\MenuBundle\Model\MenuNodeReferrersInterface

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <!-- ... -->
                <extension id="cmf_sonata_phpcr_admin_integration.menu.extension.menu_node_referrers">
                    <implements>Symfony\Cmf\Bundle\MenuBundle\Model\MenuNodeReferrersInterface</implements>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Cmf\Bundle\MenuBundle\Model\MenuNodeReferrersInterface;

        $container->loadFromExtension('sonata_admin', [
            'extensions' => [
                'cmf_sonata_phpcr_admin_integration.menu.extension.menu_node_referrers' => [
                    'implements' => [
                        MenuNodeReferrersInterface::class,
                    ],
                ],
            ],
        ]);

See the `Sonata Admin extension documentation`_ for more information.

MenuOptionInterface Sonata Admin Extension
------------------------------------------

This menu admin integration provides an extension that allows user to edit
different menu options using the Sonata admin interface.

The extension can be separately disabled and you can define the form group and
tab to be used by the field this extension adds:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                menu:
                    extensions:
                        menu_options:
                            enabled: true
                            advanced: false
                            form_group: form.group_menu_options
                            form_tab: form.tab_general

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="2.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsd="http://www.w3.org/2001/XMLSchema-instance"
            xsd:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                http://cmf.symfony.com/schema/dic/sonata_admin_integration http://cmf.symfony.com/schema/dic/sonata_admin_integration/sonata_admin_integration.xsd"
        >

            <config xmlns="http://cmf.symfony.com/schema/dic/sonata_admin_integration">
                <bundles>
                    <menu>
                        <extensions>
                            <menu-options
                                enabled="true"
                                advanced="false"
                                form-group="form.group_menu_options"
                                form-tab="form.tab_general"
                             />
                        </extensions>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'menu' => [
                    'extensions' => [
                        'menu_options' => [
                            'enabled' => true,
                            'advanced' => false,
                            'form_group' => 'form.group_menu_options',
                            'form_tag' => 'form.tab_general',
                        ],
                    ],
                ],
            ],
        ]);

``enabled``
~~~~~~~~~~~

**type**: ``bool`` **default**: ``true``

If ``false``, the extension is not loaded at all to save resources.

``form_group``
~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``form.group_menu_options``

.. include:: ../_partials/sonata_admin_form_group.rst.inc

``form_tab``
~~~~~~~~~~~~

**type**: ``string`` **default**: ``form.tab_general``

.. include:: ../_partials/sonata_admin_form_tab.rst.inc

``advanced``
~~~~~~~~~~~~

**type**: ``boolean`` **default**: ``false``

This activates advanced editing options for menu nodes. Without the extension,
the only editable options are **Display** and **Display Children**.

To enable advanced options, you need to add the BurgovKeyValueFormBundle_
to your project. Run ``composer require burgov/key-value-form-bundle``,
instantiate the bundle in the kernel and extend the template
``SonataAdminBundle:Form:form_admin_fields.html.twig`` to add:

.. code-block:: jinja

    {% block burgov_key_value_widget %}
        {{- block('sonata_type_native_collection_widget') -}}
    {% endblock %}

Once you enabled the bundle, you can enable the advanced menu options in your
configuration:

Using the extension
~~~~~~~~~~~~~~~~~~~

To activate the extension in your admin classes, define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_sonata_phpcr_admin_integration.menu.extension.menu_options:
                    implements:
                        - Symfony\Cmf\Bundle\MenuBundle\Model\MenuOptionsInterface

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config xmlns="http://sonata-project.org/schema/dic/admin">
                <!-- ... -->
                <extension id="cmf_sonata_phpcr_admin_integration.menu.extension.menu_options">
                    <implements>Symfony\Cmf\Bundle\MenuBundle\Model\MenuOptionsInterface</implements>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Cmf\Bundle\MenuBundle\Model\MenuOptionsInterface;

        $container->loadFromExtension('sonata_admin', [
            'extensions' => [
                'cmf_sonata_phpcr_admin_integration.menu.extension.menu_options' => [
                    'implements' => [
                        MenuOptionsInterface::class,
                    ],
                ],
            ],
        ]);

See the `Sonata Admin extension documentation`_ for more information.

The extension makes the following options editable (advanced options require additional
setup, see above):

 * Display;
 * Display children;
 * Menu attributes (advanced);
 * Label attributes (advanced);
 * Children attributes (advanced);
 * Link attributes (advanced).

See the `KnpMenuBundle documentation`_ for more information about the meaning
of those attributes.

.. _`Sonata Admin extension documentation`: https://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
.. _SonataDoctrinePHPCRAdminBundle: https://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
.. _`the sonata admin documentation`: https://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/reference/configuration.html
.. _`KnpMenuBundle documentation`: https://github.com/KnpLabs/KnpMenu/blob/master/doc/01-Basic-Menus.markdown#menu-attributes
.. _BurgovKeyValueFormBundle: https://github.com/Burgov/KeyValueFormBundle
