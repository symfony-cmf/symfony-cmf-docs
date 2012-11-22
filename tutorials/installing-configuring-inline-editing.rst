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
        "symfony-cmf/create-bundle": "1.0.*"
    },
    "scripts": {
        "post-install-cmd": [
            "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::initSubmodules",
            ...
        ],
        "post-update-cmd": [
            "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::initSubmodules",
            ...
        ]
    },

And then run

.. code-block:: bash

    php composer.phar update symfony-cmf/create-bundle

Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the ``registerBundle`` method

.. code-block:: php

    public function registerBundles()
    {
        $bundles = array(
            // ...

            new Symfony\Cmf\Bundle\CreateBundle\SymfonyCmfCreateBundle(),
            new FOS\RestBundle\FOSRestBundle(),
            new JMS\SerializerBundle\JMSSerializerBundle($this),
        );
        // ...
    }

Configuration
-------------
Next step is to configure the bundles.

Basic configuration, add to your application configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_create:
            phpcr_odm: true
            map:
                '<http://rdfs.org/sioc/ns#Post>': 'Symfony\Cmf\Bundle\MultilangContentBundle\Document\MultilangStaticContent'
            image:
                model_class: Symfony\Cmf\Bundle\CreateBundle\Document\Image
                controller_class: Symfony\Cmf\Bundle\CreateBundle\Controller\PHPCRImageController

If you have your own documents, add them to the mapping and place the RDFa mappings
in ``Resources/rdf-mappings`` either inside the ``app`` directory or inside any Bundle.
The filename is the full class name including namespace with the backslashes ``\\`` replaced by a dot ``.``.


Reference
---------

See :doc:`../bundles/create`