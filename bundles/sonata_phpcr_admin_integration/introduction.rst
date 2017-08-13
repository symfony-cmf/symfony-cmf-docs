.. index::
    single: Sonata; Bundles
    single: SonataPhpcrAdminIntegrationBundle

SonataPhpcrAdminIntegrationBundle
=================================

    The SonataPhpcrAdminIntegrationBundle provides admin services for the Sonata
    Admin tool. Additionally, it provides admin extensions to improve your
    custom admin services.

.. note::

    The admin integration bundle is not yet released as stable. It is still
    possible that there will be minor BC breaks until it is released.

Installation
------------

You can install this bundle `with composer`_ using the
``symfony-cmf/sonata-phpcr-admin-integration-bundle`` package on Packagist_.

As this bundle integrates the SonataDoctrinePhpcrAdminBundle_, please follow
`its official installation guide`_ to install the bundle.

After this, enable both the CmfSonataPhpcrAdminIntegrationBundle as well as the
SonataDoctrinePhpcrAdminBundle related bundles::

    // app/appKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...

            // SonataAdminBundle related
            new Sonata\CoreBundle\SonataCoreBundle(),
            new Sonata\BlockBundle\SonataBlockBundle(),
            new Knp\Bundle\MenuBundle\KnpMenuBundle(),
            new Sonata\AdminBundle\SonataAdminBundle(),

            // SonataDoctrinePhpcrAdminBundle related
            new Symfony\Cmf\Bundle\TreeBrowserBundle\CmfTreeBrowserBundle(),
            new Sonata\DoctrinePHPCRAdminBundle\SonataDoctrinePHPCRAdminBundle(),

            // CmfSonataPhpcrAdminIntegrationBundle
            new Symfony\Cmf\Bundle\SonataPhpcrAdminIntegrationBundle\CmfSonataPhpcrAdminIntegrationBundle(),
        );

        // ...

        return $bundles;
    }

Usage
-----

The integration bundle provides admins for the CMF bundles. The related
configuration section of a bundle becomes available whenever a CMF bundle is
registered in the ``AppKernel``. For instance, to enable the admin integration
for the :doc:`CmfContentBundle <../content/introduction>`, use the following
config:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_sonata_phpcr_admin_integration:
            bundles:
                content: ~

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
                    <content/>
                </bundles>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_sonata_phpcr_admin_integration', [
            'bundles' => [
                'content' => true,
            ],
        ]);

Adding Admins to the Dashboard
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To see an admin on the dashboard, configure the Sonata Admin accordingly. For
example, to add the routing admin, you would add the following configuration:

.. configuration-block::

    .. code-block:: yaml

        sonata_admin:
            dashboard:
                groups:
                    blocks:
                        label: URLs
                        items:
                            - cmf_sonata_phpcr_admin_integration.routing.route_admin

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/sonata_admin">
                <dashboard>
                    <group id="blocks"
                        label="URLs">
                        <item>cmf_sonata_phpcr_admin_integration.routing.route_admin</item>
                    </group>
                </dashboard>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('sonata_admin', array(
            'dashboard' => array(
                'groups' => array(
                    'blocks' => array(
                        'label' => 'URLs',
                        'items' => array(
                            'cmf_sonata_phpcr_admin_integration.routing.route_admin',
                        ),
                    ),
                ),
            ),
        ));

You can also embed an admin directly into other admin classes using the
``sonata_type_admin`` form type. Please refer to the `Sonata Admin documentation`_
for further information.

Configuration
-------------

The configuration is split into a section for each supported CMF bundle.
Each part is only available if the corresponding bundle is installed and
registered with the kernel.

.. toctree::

    core
    content
    menu
    routing
    routing_auto
    seo

Learn More
----------

* SonataAdminBundle_
* SonataDoctrinePhpcrAdminBundle_
* :doc:`The Sonata Admin chapter of the tutorial <../../tutorial/sonata-admin>`

.. _`with composer`: https://getcomposer.org
.. _`Packagist`: https://packagist.org/packages/symfony-cmf/sonata-admin-integration-bundle
.. _SonataDoctrinePhpcrAdminBundle: https://sonata-project.org/bundles/doctrine-phpcr-admin/1-x/doc/index.html
.. _its official installation guide: https://sonata-project.org/bundles/doctrine-phpcr-admin/1-x/doc/reference/installation.html
.. _SonataAdminBundle: https://sonata-project.org/bundles/admin/3-x/doc/index.html
.. _`Sonata Admin documentation`: https://sonata-project.org/bundles/admin/master/doc/reference/form_types.html
