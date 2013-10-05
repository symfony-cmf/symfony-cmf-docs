.. index::
    single: Installing Inline Editing

Installing and Configuring Inline Editing
=========================================

.. include:: _outdate-caution.rst.inc

The goal of this tutorial is to install and configure the inline editing
support.

This provides a solution to easily integrate with `VIE.js`_ and `create.js`_
to provide inline editing based on `RDFa`_ output.

For more information for now see the documentation of the
:doc:`CreateBundle <../bundles/create>`

.. index:: VIE.js, CreateBundle, FOSRestBundle, JMSSerializerBundle, RDFa, create.js, hallo.js

Installation
------------

Download the Bundles
~~~~~~~~~~~~~~~~~~~~

Add the following to your ``composer.json`` file:

.. code-block:: javascript

    "require": {
        ...
        "symfony-cmf/create-bundle": "1.0.*"
    },
    "scripts": {
        "post-install-cmd": [
            "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::downloadCreate",
            ...
        ],
        "post-update-cmd": [
            "Symfony\\Cmf\\Bundle\\CreateBundle\\Composer\\ScriptHandler::downloadCreate",
            ...
        ]
    },

And then run:

.. code-block:: bash

    $ php composer.phar update symfony-cmf/create-bundle

See :ref:`bundle-create-ckeditor` if you want to use "CKEditor" instead of
the default "hallo" editor.

Initialize Bundles
~~~~~~~~~~~~~~~~~~

Next, initialize the bundles in the ``AppKernel`` by adding them to the
``registerBundle`` method::

    // app/AppKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...

            new Symfony\Cmf\Bundle\CreateBundle\CmfCreateBundle(),
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
        cmf_create:
            phpcr_odm: true
            map:
                '<http://rdfs.org/sioc/ns#Post>': 'Symfony\Cmf\Bundle\MultilangContentBundle\Document\MultilangStaticContent'
            image:
                model_class: Symfony\Cmf\Bundle\CreateBundle\Document\Image
                controller_class: Symfony\Cmf\Bundle\CreateBundle\Controller\PHPCRImageController

If you have your own documents, add them to the mapping and place the RDFa
mappings in ``Resources/rdf-mappings`` either inside the ``app`` directory or
inside any Bundle.  The filename is the full class name including namespace
with the backslashes ``\\`` replaced by a dot ``.``.

.. _`VIE.js`: http://viejs.org
.. _`create.js`: http://createjs.org
.. _`RDFa`: http://rdfa.info
