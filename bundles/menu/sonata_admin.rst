.. index::
    single: Menu; MenuAdmin

Menu Admin with Sonata PHPCR-ODM
================================

If the SonataDoctrinePHPCRAdminBundle_ is loaded in the application kernel,
menu nodes and documents can be administrated in the Sonata admin panel. For
instructions on how to configure Sonata, see `the sonata admin documentation`_.

The ``use_sonata_admin`` option configures whether to enable the menu admin.
This defaults to ``auto``, meaning it will be enabled when the
SonataAdminBundle is registered. Other values are ``true`` and ``false``,
forcing the admin to respectively be enabled or disabled.

The menu admin uses the ``content_basepath`` to show the tree of nodes to
select the menu node target.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_menu:
            persistence:
                phpcr:
                    # use true/false to force using / not using sonata admin
                    use_sonata_admin: auto

                    # used with Sonata Admin to manage content; defaults to %cmf_core.basepath%/content
                    content_basepath: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <persistence>
                    <!-- use-sonata-admin: use true/false to force using / not using sonata admin -->
                    <!-- content-basepath: used with Sonata Admin to manage content;
                                           defaults to %cmf_core.basepath%/content -->
                    <phpcr
                        use-sonata-admin="auto"
                        content-basepath="null"
                    />
                </persistence>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_menu', array(
            'persistence' => array(
                'phpcr' => array(
                    // use true/false to force using / not using sonata admin
                    'use_sonata_admin' => 'auto',

                    // used with Sonata Admin to manage content; defaults to %cmf_core.basepath%/content
                    'content_basepath' => null,
                ),
            ),
        ));


MenuNodeReferrersInterface Sonata Admin Extension
-------------------------------------------------

This bundle provides an extension to edit referring menu nodes for content that
implements the ``MenuNodeReferrersInterface``.

To enable the extensions in your admin classes, simply define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_menu.admin_extension.menu_node_referrers:
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
                <extension id="cmf_menu.admin_extension.menu_node_referrers">
                    <implement>Symfony\Cmf\Bundle\MenuBundle\Model\MenuNodeReferrersInterface</implement>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            'extensions' => array(
                'cmf_menu.admin_extension.menu_node_referrers' => array(
                    'implements' => array(
                        'Symfony\Cmf\Bundle\MenuBundle\Model\MenuNodeReferrersInterface',
                    ),
                ),
            ),
        ));

See the `Sonata Admin extension documentation`_ for more information.

MenuOptionInterface Sonata Admin Extension
------------------------------------------

This bundle provides an extension that allows user to edit different menu
options using the Sonata admin interface.

To enable the extensions in your admin classes, simply define the extension
configuration in the ``sonata_admin`` section of your project configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_admin:
            # ...
            extensions:
                cmf_menu.admin_extension.menu_options:
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
                <extension id="cmf_menu.admin_extension.menu_options">
                    <implement>Symfony\Cmf\Bundle\MenuBundle\Model\MenuOptionsInterface</implement>
                </extension>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_admin', array(
            'extensions' => array(
                'cmf_menu.admin_extension.menu_options' => array(
                    'implements' => array(
                        'Symfony\Cmf\Bundle\MenuBundle\Model\MenuOptionsInterface',
                    ),
                ),
            ),
        ));

See the `Sonata Admin extension documentation`_ for more information.

These are the list of available options:

 * Display;
 * Display children;
 * Menu attributes (advanced);
 * Label attributes (advanced);
 * Children attributes (advanced);
 * Link attributes (advanced).

See the `KnpMenuBundle documentation`_ for more information about these
attributes.

Advanced Menu Options
~~~~~~~~~~~~~~~~~~~~~

By default the only available options are **Display** and **Display Children**.
You can enable advanced options, but need to add the BurgovKeyValueFormBundle_
to your project. Run ``composer require burgov/key-value-form-bundle``,
instantiate the bundle in the kernel and extend the template
``SonataAdminBundle:Form:form_admin_fields.html.twig`` to add:

.. code-block:: jinja

    {% block burgov_key_value_widget %}
        {{- block('sonata_type_native_collection_widget') -}}
    {% endblock %}

Once you enabled the bundle, you can enable the advanced menu options in your
configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_menu:
            admin_extensions:
                menu_options:
                    advanced: true


    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <admin-extensions>
                    <menu-options advanced="true">
                </admin-extensions>
            </config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_menu', array(
            'admin_extensions' => array(
                'menu_options' => array(
                    'advanced' => true,
                ),
            ),
        ));

.. _`Sonata Admin extension documentation`: https://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
.. _SonataDoctrinePHPCRAdminBundle: https://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
.. _`the sonata admin documentation`: https://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/reference/configuration.html
.. _`KnpMenuBundle documentation`: https://github.com/KnpLabs/KnpMenu/blob/master/doc/01-Basic-Menus.markdown#menu-attributes
.. _BurgovKeyValueFormBundle: https://github.com/Burgov/KeyValueFormBundle
