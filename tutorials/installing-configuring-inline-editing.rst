Installing and configuring inline editing
=========================================
The goal of this tutorial is to install and configure the inline editing support.
Note that Symfony2.1 (currently master) is required for the CMF to work.

This provides a solution to easily integrate with `VIE.js <http://viejs.org>`_ and `create.js <http://createjs.org>`_
to provide inline editing based on `RDFa <http://rdfa.info>`_ output.

For more information for now see the documentation of the `LiipVieBundle <https://github.com/liip/LiipVieBundle#readme>`_

.. index:: VIE.js, LiipVieBundle, FOSRestBundle, JMSSerializerBundle, RDFa, create.js, hallo.js

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file

.. code-block:: javascript

    "require": {
        ...
        "liip/vie-bundle": "dev-master"
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

And then run

.. code-block:: bash

    php composer.phar update

Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the ``registerBundle`` method

.. code-block:: php

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

Basic configuration, add to your application configuration

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_create:
            phpcr_odm: true
            map:
                '<http://rdfs.org/sioc/ns#Post>': 'Sandbox\MainBundle\Document\EditableStaticContent'
            rdf_config_dirs:
                - %kernel.root_dir%/Resources/rdf-mappings

Reference
---------

See :doc:`/reference/create`