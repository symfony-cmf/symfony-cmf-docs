.. index::
    single: Menu; MenuAdmin

Menu Admin with Sonata PHPCR-ODM
================================

If the SonataDoctrinePHPCRAdminBundle_ is loaded in the application kernel,
menu node and menu documents can be administrated in Sonata admin. For
instructions on how to configure Sonata, see `configuring sonata admin`_.

Loading of Sonata is controlled with the ``use_sonata_admin`` configuration
setting. By default, this option is automatically set based on whether
SonataDoctrinePHPCRAdminBundle is available, but you can explicitly
disable the flag to not provide the Sonata services even if Sonata would be
available. You can also explicitly enable the flag to get an error if Sonata
becomes unavailable.

Sonata admin is using the ``content_basepath`` to show the tree of content to
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

.. _`Sonata Admin extension documentation`: http://sonata-project.org/bundles/admin/master/doc/reference/extensions.html
.. _SonataDoctrinePHPCRAdminBundle: http://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/index.html
.. _`configuring sonata admin`: http://sonata-project.org/bundles/doctrine-phpcr-admin/master/doc/reference/configuration.html
