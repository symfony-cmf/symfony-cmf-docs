Installing and configuring inline editing
=========================================
The goal of this tutorial is to install and configure the inline editing support.
Note that Symfony2.1 (currently master) is required for the CMF to work.

This provides a solution to easily integrate with VIE.js and create.js to provide inline editing
based on RDFa output.

Preconditions
-------------
- :doc:`/tutorials/installing-configuring-cmf`
- Or - :doc:`/reference/routing-extra`, :doc:`/reference/menu` and :doc:`/reference/block` need to be installed manually

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file::

    "require": {
        ...
        "symfony-cmf/liip-vie-bundle": "1.0.*"
    },
    "scripts": {
        "post-install-cmd": [
            "Liip\\VieBundle\\Composer\\ScriptHandler::initSubmodules",
            ...
        ],
        "post-update-cmd": [
            "Liip\\VieBundle\\Composer\\ScriptHandler::initSubmodules",
            ...
        ]
    },

And then run::

    php composer.phar update

Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the ``registerBundle`` method::

    public function registerBundles()
    {
        $bundles = array(
            // ...

            new Liip\VieBundle\LiipVieBundle(),
            new FOS\RestBundle\FOSRestBundle(),
            new JMS\SerializerBundle\JMSSerializerBundle($this),

        );
        // ...
    }
    
Configuration
-------------
Next step is to configure the bundles.

Basic configuration, add to ``app/config/config.yml``::

    liip_vie:
        phpcr_odm: true
        map:
            '<http://rdfs.org/sioc/ns#Post>': 'Sandbox\MainBundle\Document\EditableStaticContent'
        use_coffee: %liip_vie.use_coffee%
        base_path: /cms/routes
        cms_path: /cms/content/static
            
More information on configuring this bundle can be found `here <https://github.com/symfony-cmf/SimpleCmsBundle#readme>`_.
