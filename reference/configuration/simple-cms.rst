SimpleCms Configuration
===========================

The SimpleCmsBundle provide a simple CMS solution and can be configured under
the ``cmf_simple_cms`` key in your application configuration. When using
XML, you can use the ``http://cmf.symfony.com/schema/dic/simplecms`` namespace.

Configuration
-------------

persistence
~~~~~~~~~~~

.. _config-persistence:


phpcr
.....

This defines the persistence driver. The default configuration of persistence 
is the following configuration :

.. configuration-block::

    .. code-block:: yaml

        cmf_simple_cms:
            phpcr:
                enabled: false
                basepath: /cms/simple
		    manager_registry: doctrine_phpcr
		    manager: 
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


enabled
*******

**type**: ``boolean`` **default**: ``false``

If ``true``, PHPCR is enabled in the service container.

basepath
**************

**type**: ``string`` **default**: ``/cms/simple``

The basepath for CMS documents in the PHPCR tree.

manager_registry
**************

**type**: ``string`` **default**: ``doctrine_phpcr``

manager_name
************

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use.

If the :doc:`CoreBundle <../../bundles/core>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.enabled``.

document_class
*******

**type**: ``string`` **default**: ``'Symfony\Cmf\Bundle\SimpleCmsBundle\Doctrine\Phpcr\Page'``

The class for the pages.

use_sonata_admin
****************

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

