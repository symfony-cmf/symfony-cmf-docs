.. index::
    single: Testing
    single: Components; Testing

Testing
=======

The Testing compoent is an **internal tool** for testing Symfony CMF bundles.
It provides a way to easily bootstrap a consistent functional test environment.

Data Fixtures
-------------

Configuration
-------------

Your Test Application
---------------------

(my opinion): As much as is possible Bundles should be developed in isolation
from other bundles, that is, they should not be developed in the context of
the `cmf-sandbox`, or your own application, but instead on their own.

This may not always be possible, and may indeed be difficult when developing
graphical elements, but, in general, this is good advice. By developing your
bundle exclusively from the context of a functional test POV you are
constantly reasserting the work that you have previously completed.

Your functional test application should be an application which implements
your bundle to its full extent.

You test application must be structured as follows:

.. code-block::

    ./Tests
        ./Functional/
            ./App/
                ./Kernel
                    ./AppKernel.php
                    ./config/
                        default.php
                ./Document # optional
                    ./CustomDocument1.php
                    ./CustomDocument2.php


AppKernel
~~~~~~~~~

Talk about AppKernel here.

Documents
~~~~~~~~~

The testing component will automatically include PHPCR-ODM documents (Entity's
and other types of persistant objects can be added later) in the PHPCR-ODM
configuration **only if** the documents are placed in
`Tests/Functional/App/Document`.

Configuration
~~~~~~~~~~~~~

The testing component includes some pre-defined configurations to get things
going with a minimum of effort and repetition.

Because of the apparent difficulty in getting the directory of these
configurations we have had to define this directory as a *PHP constant*.
Thusly, to implement the default configurations create the following PHP file:

.. code-block:: php

    // Tests/Functional/App/Kernel/config/config.php
    <?php

    $loader->import(CMF_TEST_CONFIG_DIR.'/default.php');
    $loader->import(__DIR__.'/mybundleconfig.yml');

Here you include the testing components **default** configuration, which will
basically get everything working with a minimal configuration. Then you can
optionally import configurations specific to your bundle.

Setting up PHPUnit
~~~~~~~~~~~~~~~~~~

Explain about `KERNEL_DIR`. Show example phpunit.dist.xml
