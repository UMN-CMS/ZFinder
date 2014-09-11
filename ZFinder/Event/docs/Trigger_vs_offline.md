# Trigger Cuts Vs. Offline Cuts

The offline cut is for the tighter of the two electrons we select.

| Cut                            | HLT EE | HLT EB | Tight Offline EE | Tight Offline EB |
|--------------------------------|--------|--------|------------------|------------------|
| abs(eta) <                     | 2.65   | 1.4791 | 2.1              | 2.1              |
| pT >                           | 27     | 27     | 30               | 30               |
| sigma ieta ieta <              | 0.03   | 0.01   | 0.03             | 0.01             |
| ECAL Iso / E <                 | 0.1    | 0.15   | —                | —                |
| H/E <                          | 0.05   | 0.1    | 0.1              | 0.12             |
| HCAL Iso / E <                 | 0.1    | 0.1    | —                | —                |
| Pixel Matching >=              | 1 (3)  | 1 (3)  | —                | —                |
| 1/E - 1/p <                    | 0.05   | 0.05   | 0.05             | 0.05             |
| dEta <                         | 0.007  | 0.007  | 0.005            | 0.004            |
| dPhi <                         | 0.03   | 0.06   | 0.02             | 0.03             |
| Conversion Reject Missing Hits | —      | —      | 0                | 0                |
| abs(dZ vtx) <                  | —      | —      | 0.1              | 0.1              |
| abs(d0 vtx) <                  | —      | —      | 0.02             | 0.02             |
| PF Isolation / pT <            | —      | —      | 0.1              | 0.1              |

In the case of pixel matching for HLT, the Twiki says 3 but the code seems to
indicate 1.
