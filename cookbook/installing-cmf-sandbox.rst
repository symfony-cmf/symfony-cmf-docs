Installing the CMF sandbox
==========================

This tutorial shows how to install the Symfony CMF Sandbox, a demo platform
aimed at showing the tool's basic features running on a demo environment.
This can be used to evaluate the platform or to see actual code in action,
helping you understand the tool's internals.

While it can be used as such, this sandbox does not intend to be a development
platform. If you are looking for installation instructions for a development
setup, please refer to:

- The `Symfony2 CMF standard edition <https://github.com/symfony-cmf/symfony-cmf-standard>`_ page for instructions on how to quickly install the CMF (recommended for development)
- `Installing and configuring the CMF <http://symfony.com/doc/master/cmf/tutorials/installing-configuring-cmf.html>`_ for step-by-step installation and configuration details (if you want to know all the details)

.. In the future, split between fast and detailed installations.

Preconditions
-------------

As Symfony CMF Sandbox is based on Symfony 2, you should make sure you
meet the `Requirements for running Symfony2 <http://symfony.com/doc/current/reference/requirements.html>`_.
Aditionally, you need to have `Sqlite <http://www.sqlite.org/>`_ 
installed, since it's used as the sandbox's default storage medium.

`Git <http://git-scm.com/>`_ and `Curl <http://curl.haxx.se/>`_ are also needed to follow the installation steps listed below.


Installation
------------

Getting the code
~~~~~~~~~~~~~~~~

The Symfony CMF Sandbox source code is available on github. To get it use

.. code-block:: bash

    git clone git://github.com/symfony-cmf/symfony-cmf-standard.git

Move into the folder and fetch composer

.. code-block:: bash

    cd symfony-cmf-standard
    curl -s http://getcomposer.org/installer | php --
    
Install all the required bundles (this may take a while)

.. code-block:: bash

    php composer.phar install

Next, create the Sqlite database:

.. code-block:: bash

    app/console doctrine:database:create
    app/console doctrine:phpcr:init:dbal
    app/console doctrine:phpcr:register-system-node-types
    app/console doctrine:phpcr:fixtures:load

This will create a file called app.sqlite inside your app folder, containing
the database content needed by the sandbox.

The sandbox should now be accessible on your web server.

.. code-block:: text

    http://localhost/
    
You can also access the default content management panel by pointing your
browser to:

.. code-block:: text

    http://localhost/admin/dashboard