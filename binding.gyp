{
  "targets": [
    {
      "target_name": "winregistry",
      "conditions": [
        ['OS=="win"', {
          "sources": [
            "src/winregistry.cc"
          ]
        }]
      ]
    }
  ]
}
