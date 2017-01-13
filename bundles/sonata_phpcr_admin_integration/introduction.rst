.. index::
    single: Sonata; Bundles
    single: SonataPhpcrAdminIntegrationBundle

SonataPhpcrAdminIntegrationBundle
=================================

    The SonataPhpcrAdminIntegrationBundle provides admin services for the Sonata
    Admin tool. Additionally, it provides admin extensions to improve your
    custom admin services.

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

Sonata requires the ``sonata_block`` bundle to be configured in your main
configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml

        # ...
        sonata_block:
            default_contexts: [cms]
            blocks:
                # Enable the SonataAdminBundle block
                sonata.admin.block.admin_list:
                    contexts: [admin]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="htp://symfony.com/schema/dic/services">
            <config xmlns="http://sonata-project.org/schema/dic/block">
                <default-context>cms</default-context>

                <block id="sonata.admin.block.admin_list">
                    <context>admin</context>
                </block>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_block', array(
            'default_contexts' => array('cms'),
            'blocks' => array(
                'sonata.admin.block.admin_list' => array(
                    'contexts' => array('admin'),
                ),
            ),
        ));

and it requires the following entries in your routing file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml

        admin:
            resource: '@SonataAdminBundle/Resources/config/routing/sonata_admin.xml'
            prefix: /admin

        _sonata_admin:
            resource: .
            type: sonata_admin
            prefix: /admin

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import
                resource="@SonataAdminBundle/Resources/config/sonata_admin.xml"
                prefix="/admin"
            />

            <import
                resource="."
                type="sonata_admin"
                prefix="/admin"
            />

        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $routing = $loader->import(
            "@SonataAdminBundle/Resources/config/sonata_admin.xml"
        );
        $routing->setPrefix('/admin');
        $collection->addCollection($routing);

        $_sonataAdmin = $loader->import('.', 'sonata_admin');
        $_sonataAdmin->addPrefix('/admin');
        $collection->addCollection($_sonataAdmin);

        return $collection;

and publish your assets (remove ``--symlink`` if you use Windows!):

.. code-block:: bash

    $ php bin/console assets:install --symlink web/

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

Configuration
-------------

This section documents all available admin integrations in this bundle, with
their configuration options.

Block
~~~~~

Content
~~~~~~~

This integration becomes available once the :doc:`CmfContentBundle
<../content/introduction>` is installed. This will provide an admin interface
for the ``StaticContent`` document. Enable this admin using:

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

.. tip::

    Install the IvoryCKEditorBundle_ to enable a CKEditor to edit the content
    body:

    .. code-block:: bash

        $ composer require egeloen/ckeditor-bundle

Core
~~~~

Menu
~~~~

Routing
~~~~~~~

RoutingAuto
~~~~~~~~~~~

Seo
~~~

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
.. _IvoryCKEditorBundle: https://github.com/egeloen/IvoryCKEditorBundle
