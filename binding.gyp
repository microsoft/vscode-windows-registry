{
  "targets": [
    {
      "target_name": "winregistry",
      "conditions": [
        ['OS=="win"', {
          "sources": [
            "src/winregistry.cc"
          ],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'AdditionalOptions': [
                '/W3',
                '/Qspectre',
                '/guard:cf',
                '/we4244',
                '/we4267',
                '/ZH:SHA_256'
              ]
            },
            'VCLinkerTool': {
              'AdditionalOptions': [
                '/guard:cf'
              ]
            }
          },
        }]
      ]
    }
  ]
}
