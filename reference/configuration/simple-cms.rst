SimpleCmsBundle Configuration
=============================

The SimpleCmsBundle provide a simple CMS solution and can be configured under
the ``cmf_simple_cms`` key in your application configuration. When using
XML, you can use the ``http://cmf.symfony.com/schema/dic/simplecms`` namespace.

Configuration
-------------

.. _config-persistence:

persistence
~~~~~~~~~~~

phpcr
.....

This defines the persistence driver. The default configuration of persistence 
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            phpcr:
                enabled: false
                basepath: /cms/simple
                manager_registry: doctrine_phpcr
                manager: ~
                document_class: Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page
                use_sonata_admin: auto
                sonata_admin:
                    sort: false
		    
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

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/simple_cms"
                enabled="false"
                basepath="/cms/simple"
                manager-registery="doctrine_phpcr"
                manager="null"
                document-class="Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page"
                use-sonata-admin="auto"
            >
                <sonata-admin sort="false" />
            </config>

        </container>


enabled
"""""""

**type**: ``boolean`` **default**: ``false``

If ``true``, PHPCR is enabled in the service container.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.enabled``.

PHPCR can be enabled by multiples ways such as:

.. configuration-block::

    .. code-block:: yaml

        phpcr: ~ # use default configuration
        # or
        phpcr: true # straight way
        # or
        phpcr:
            manager: ... # or any other option under 'phpcr'

basepath
""""""""

**type**: ``string`` **default**: ``/cms/simple``

The basepath for CMS documents in the PHPCR tree.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.basepath``.

manager_registry
""""""""""""""""

**type**: ``string`` **default**: ``doctrine_phpcr``

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.manager_registry``.

manager_name
""""""""""""

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.manager_name``.

document_class
""""""""""""""

**type**: ``string`` **default**: ``'Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page'``

The class for the pages.

use_sonata_admin
""""""""""""""""

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the admin classes for the pages are activated on the sonata
admin panel. If set to ``auto``, the admin services are activated only if the
SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to the value
of ``cmf_core.persistence.phpcr.use_sonata_admin``.


use_menu
~~~~~~~~

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

.. _config-use_menu:

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            use_menu: auto

    .. code-block:: php

        $container->loadFromExtension('simple_cms', array(
            'use_menu' => 'auto'
        ));

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/simple_cms"
                use-menu="auto"
            />
        </container>

routing
~~~~~~~

.. _config-routing:

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
                uri_filter_regexp:

    .. code-block:: php

        $container->loadFromExtension('simple_cms', array(
            'routing' => array(
                'controller_by_alias' => array(),
                'controller_by_class' => array(),
                'templates_by_class' => array(
                    'Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page' => 'CmfSimpleCmsBundle:Page:index.html.twig',
                ),
                'generic_controller' => 'cmf_content.controller:indexAction',
                'content_repository_id' => 'cmf_routing.content_repository',
                'uri_filter_regexp' => '',
            ),
        ));

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <routing xmlns="http://cmf.symfony.com/schema/dic/simple_cms">
                <controller-by-alias></controller-by-alias>
                <controller-by-class></controller-by-class>
                <template-by-class alias="Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page">CmfSimpleCmsBundle:Page:index.html.twig</template-by-class>
                <generic-controller>cmf_content.controller:indexAction</generic-controller>
                <content-repository-id>cmf_routing.content_repository</content-repository-id>
                <uri-filter-regexp></uri-filter-regexp>
            </routing>
        </container>
                

multilang
~~~~~~~~~

.. _config-multilang:

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            multilang:
                locales: [en, fr]
                
locales
.......

**type**: ``array`` **default**: ``null``

This define languages that can be used. 

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to
the value of ``cmf_core.multilang.locales``.

Multilanguage is activated if the ``locales`` option is configured either in 
SimpleCmsBundle or in CoreBundle.
