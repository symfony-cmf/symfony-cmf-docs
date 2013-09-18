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

**prototype**: ``array`` **default**: ``{ enabled: false }``

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
